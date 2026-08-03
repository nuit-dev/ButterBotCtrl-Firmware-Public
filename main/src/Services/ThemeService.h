#ifndef BUTTERBOTCTRL_FIRMWARE_THEMESERVICE_H
#define BUTTERBOTCTRL_FIRMWARE_THEMESERVICE_H

#include <Core/Application.h>
#include <misc/lv_color.h>
#include <vector>
#include <string>
#include <FileSystem/ArchiveCache.h>
#include <FileSystem/CompositeCache.h>

#include "Themes.hpp"

class RawCache;

struct ThemeStyle {
	lv_color_t primaryColor;
	lv_color_t secondaryColor;
	lv_color_t tertiaryColor;
};

typedef std::unordered_map<Theme, const ThemeStyle*> ThemedStylesMap;

class ThemeService : public Object {
	GENERATED_BODY(ThemeService, Object, CONSTRUCTOR_PACK(Theme))

public:
	/**
	 * @brief Default constructor. If no Theme enum is passed, uses the main theme as default.
	 */
	ThemeService(Theme theme = Theme::Main);

	~ThemeService() override;

	/**
	 * @brief Set/change the current theme.
	 */
	void setTheme(Theme theme);

	/**
	 * @brief Get current theme.
	 */
	Theme getTheme() const;

	/**
	 * @return Style of the current theme.
	 */
	const ThemeStyle& getThemeStyle() const;

	/**
	 * @return Primary color for the current theme.
	 */
	lv_color_t getPrimaryColor() const;

	/**
	 * @return Secondary color for the current theme.
	 */
	lv_color_t getSecondaryColor() const;

	/**
	 * @return Secondary color for the current theme.
	 */
	lv_color_t getTertiaryColor() const;

	/**
	 * @brief Fetch an asset for the currently set theme.
	 * @return Absolute path to the asset.
	 */
	const char* getAsset(Asset asset) const;

	/**
	 * @brief Load the intro GIF archive and clear all other caches.
	 */
	void activateIntroAssets();

	/**
	 * @brief Load the pairing GIF archive and clear all other caches.
	 */
	void activatePairingAssets();

	/**
	 * @brief Load the current theme archive.
	 */
	void activateThemeAssets();

	FileCache* getCache() const;

private:
	Theme currentTheme;
	static constexpr size_t NumCaches = 3;

	enum class ActiveAssets { None, Intro, Pairing, Theme };
	ActiveAssets activeAssets = ActiveAssets::None;

	ArchiveCache* animCache = nullptr; // Intro or Pairing GIFs
	RawCache* rawCache = nullptr; // Most commonly used UI elements
	ArchiveCache* archiveCache = nullptr; // Current theme archive

	CompositeCache<NumCaches>* compositeCache = nullptr;

	// PSRAM buffer reserved once at boot and shared by archive caches
	static constexpr size_t ArchiveBufferSize = 512 * 1024;
	uint8_t* archiveBuffer = nullptr;

	std::vector<std::string> buildRawCachePaths() const;

	/**
	 * @brief Load startup GIF archive into the shared buffer
	 */
	void loadStartupArchive(const char* archivePath, ActiveAssets target);

	/**
	 * @brief Decompress the current theme archive into the caches
	 */
	void loadThemeArchive();

};

#endif //BUTTERBOTCTRL_FIRMWARE_THEMESERVICE_H
