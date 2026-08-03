# archive.sh

Packs groups of files from `spiffs_archives/` into a custom binary archive format (`.sz` files) written to `spiffs_image/`. These archives are read at runtime by `FileArchive.cpp` in the CMF framework, which loads them from SPIFFS into RAM and serves individual files from them without hitting the filesystem per-file.

**Usage:** `./scripts/archive.sh [--nocompress]`

---

## Shell setup

```bash
set -e               # abort the whole script on any command failure
shopt -s globstar    # enables ** to match files recursively in subdirectories
shopt -s extglob     # enables extended glob patterns (not actually used here, but available)

compress=true
for arg in "$@"; do
  if [[ "$arg" == "--nocompress" ]]; then
    compress=false
  fi
done
```

The script accepts one optional argument: `--nocompress`. When passed, the heatshrink compression step is skipped and the plain `.sz` file is kept instead of producing a `.sz.hs` file. Useful for inspecting the raw archive contents during development.

---

## Outer loop — one archive per subdirectory

```bash
for arch in spiffs_archives/*; do
  name=$(basename $arch)          # e.g. "t_1", "pairing"
  data="spiffs_image/$name.sz"    # output path, e.g. "spiffs_image/t_1.sz"
  rm -f $data                     # delete any previous build artifact cleanly
```

Each subdirectory of `spiffs_archives/` becomes one `.sz` archive file. For example, `spiffs_archives/t_1/` → `spiffs_image/t_1.sz`.

---

## Count the files

```bash
  files=("$arch/**/*")    # glob all paths (files + dirs) recursively under this archive dir

  count=0
  for file in "${files[@]}"; do
      if [[ -f $file ]]; then   # skip directories, count only regular files
        count=$((count+1))
      fi
  done
```

This counts only regular files (not directories). The count is written first because `FileArchive.cpp` reads it at offset 0 to `reserve()` its entry list.

---

## Write the file count (little-endian uint32)

```bash
  printf "0: %.8x" $count \
    | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' \
    | xxd -r -g0 >> $data
```

This is the little-endian serialization pipeline used throughout the script:

1. `printf "0: %.8x" $count` — formats the number as 8 hex nibbles with an `xxd`-style `offset: hex` prefix, e.g. `0: 00000003`
2. `sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/'` — reverses the 4 byte-pairs to convert big-endian hex to little-endian, e.g. `0: 03000000`
3. `xxd -r -g0` — converts the hex string back to raw bytes and appends them to the output file

The ESP32 is little-endian, and `FileArchive.cpp:11` reads the count with a plain `file.read((uint8_t*) &count, 4)`, so this encoding must match.

---

## Write the table of contents (directory section)

```bash
  for file in "${files[@]}"; do
    if [[ -d $file ]]; then continue; fi   # skip directories

    # Strip the "spiffs_archives/<archiveName>" prefix — leaves e.g. "/icons/foo.bin"
    name=$(sed 's/spiffs_archives\/[a-zA-Z0-9]*//' <<< $file)
    size=$(du -b $file | cut -f1)   # exact byte size of the file

    printf "%s\0" $name >> $data    # null-terminated filename string
    printf "0: %.8x" $size \
      | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' \
      | xxd -r -g0 >> $data          # 4-byte little-endian file size
  done

  printf "\0" >> $data               # null byte terminates the directory section
```

This builds the **index/directory** of the archive — a sequence of `(name\0, size_u32)` pairs, terminated by an empty null byte. `FileArchive.cpp` reads this in its `while(file.available())` loop: it reads bytes until `\0` to get the filename, then reads 4 bytes for the size, and breaks when it encounters an empty name (the terminating `\0`).

---

## Write the raw file data

```bash
  for file in "${files[@]}"; do
    if [[ -d "$file" ]]; then continue; fi

    cat $file >> $data   # append raw bytes of each file, in the same order as the directory
  done
```

After the directory, all file contents are concatenated in the same iteration order as the directory was written. `FileArchive.cpp` reads them sequentially, using the sizes from the directory to know how many bytes each file occupies, and records the in-RAM offset of each one.

---

## Resulting `.sz` binary format

```
[ 4 bytes ]   file count (little-endian uint32)
  [ N bytes ]   filename\0  (null-terminated string)
  [ 4 bytes ]   file size   (little-endian uint32)
  ... repeated for each file ...
[ 1 byte  ]   \0           (terminates directory)
[ ? bytes ]   raw file data (file 1 bytes, file 2 bytes, ...)
```

---

## Compress and discard the uncompressed archive

```bash
  if [[ "$compress" == true ]]; then
    heatshrink -e -w 14 -l 7 "$data" "$data.hs"
    rm "$data"
  fi
```

Unless `--nocompress` was passed, the completed `.sz` file is compressed with [heatshrink](https://github.com/atomicobject/heatshrink) — a lightweight LZ77-based compressor designed for embedded systems with very little RAM overhead during decompression.

- `-e` — encode (compress)
- `-w 14` — window size of 2¹⁴ = 16 384 bytes; larger windows find more repeated patterns at the cost of more RAM during decompression
- `-l 7` — lookahead of 2⁷ = 128 bytes; controls how far ahead the encoder searches for matches

The output file is named by appending `.hs` to the original path, e.g. `spiffs_image/t_1.sz.hs`. The intermediate `.sz` file is then deleted because only the compressed version is needed on the device. When `--nocompress` is active, neither of these steps runs and the `.sz` file is left in place.

---


## How it fits into the firmware

At runtime, `ArchiveCache::load()` opens uncompressed `<path>.sz` from SPIFFS, passes it to 
`FileArchive`, which reads the whole thing into a single `malloc`'d buffer and builds a `name → {offset, size}` hash map. Individual files are then served as `RamFile` handles pointing into that buffer — avoiding per-file SPIFFS overhead, which is significant on embedded hardware.