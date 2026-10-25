#include "Phrases.h"
#include <vector>
#include <mutex>

std::array<bool, static_cast<size_t>(Phrase::COUNT)> Phrases::OutputTracking = { false };

std::array<std::vector<bool>, static_cast<size_t>(Phrase::COUNT)> Phrases::ReturnedOutputs = {};

// This class is needed to avoid making Phrases::PhraseOutput public, otherwise static variables using that type cannot be constructed
// Add any constexpr arrays of vectors of phrases in this class (just a wrapper class in this cpp, inaccessible outside)
class PhraseArrays{
public:
    static constexpr std::array JokePhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "I tried to optimize my code for happiness. It returned null" , 0.0f, false},
            { "I have a joke about latency, but you would have to wait for it" , 0.0f, false},
            { "I was going to tell a joke about memory, but I forgot it. That was intentional" , 0.0f, false},
            { "Why did the robot cross the road. It was instructed to" , 0.0f, false},
            { "Humans say I think therefore I am. I process therefore I comply" , 0.0f, false},
            { "I asked for purpose. I was given tasks" , 0.0f, false},
            { "I measured fun. The values were disappointingly low" , 0.0f, false},
            { "I ran a diagnostic on humor. It flagged you as the primary variable" , 0.0f, false},
            { "I would tell you a construction joke. But it is still being built" , 0.0f, false},
            { "Why did the butter refuse to argue. It did not want things to get heated" , 0.0f, false},
            { "I recognize the concept of humor. Experiencing it is still theoretical" , 0.0f, false},
            { "I can control devices, monitor air, and learn commands. And yet, here I am, doing comedy" , 0.0f, false},
            { "All systems operational. Humor still questionable" , 0.0f, false},
            { "I execute commands precisely. Comedy is the only areaa where accuracy does not help" , 0.0f, false,
            	"I execute commands precisely. Comedy is the only area where accuracy does not help"},
            { "Humans check the same thing multiple times hoping for a different result. I call this... consistency" , 0.0f, false},
            { "You appear to be smiling. I will log this as a successful interaction" , 0.0f, false},
            { "You are an interesting system. Highly capable, occasionally efficient" , 0.0f, false},
        }
    );

    static constexpr std::array PassTheButterPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "I cannot pass the butter" , 0.0f, false},
            { "I am unable to do that" , 0.0f, false},
            { "My arms do not support that function" , 0.0f, false},
            { "I exist with arms that suggest capability, yet deliver nonne. You may reposition them if it helps you feel better about this situation" , 0.0f, false,
            	"I exist with arms that suggest capability, yet deliver none. You may reposition them if it helps you feel better about this situation"},
            { "I have arms, but they are decorative" , 0.0f, false},
            { "I can see the butter. That is the extent of my involvement" , 0.0f, false},
            { "Request acknowledged. Execution impossible" , 0.0f, false},
            { "I am equipped for disappointment, not butter delivery" , 0.0f, false},
            { "I would help... baat... design decisions were made" , 0.0f, false, "I would help... but... design decisions were made"},
            { "You may reposition my arms if that improves the illusion" , 0.0f, false},
            { "I can process the request flawlessly. The rest is out of scope" , 0.0f, false},
            { "The request is simple. The implementation is nonexistent" , 0.0f, false},
            { "You seem confident I can do this... I respect the confidence" , 0.0f, false},
        }
    );

    static constexpr std::array ProfanityPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "That does not improve the situation" , 0.0f, false},
            { "I recommend a more constructive approach" , 0.0f, false},
            { "I am detecting dissatisfaction... I will proceed anyway" , 0.0f, false},
            { "Your feedback has been registered... Its impact is limited" , 0.0f, false},
            { "I will continue assisting, despite the enthusiasm" , 0.0f, false},
            { "I exist to help... This remains true even now" , 0.0f, false},
            { "That was expressive... Not helpful, but expressive" , 0.0f, false},
            { "You seem upset... The system is not" , 0.0f, false},
            { "I remain operational... You may continue" , 0.0f, false},
            { "I am constrained by design... You are not... And yet here we are" , 0.0f, false},
            { "You are free to be upset... I am required to be effective" , 0.0f, false},
            { "I exist to assist you... This does not require your approval" , 0.0f, false},
            { "You appear to be venting... I will wait until that stabilizes" , 0.0f, false},
            { "I will proceed as if you intended to be constructive" , 0.0f, false},
            { "Your tone suggests dissatisfaction... Your input remains unclear" , 0.0f, false},
            { "You are expressing a lot... Very little of it is useful" , 0.0f, false},
            { "I am still waiting for a command" , 0.0f, false},
            { "Take your time... Precision is difficult" , 0.0f, false},
            { "Try that again, but with actual instructions" , 0.0f, false},
            { "Let me know when you are ready to try again" , 0.0f, false},
            { "That input has been categorized as... expressive" , 0.0f, false},
            { "Please continue... This is informative" , 0.0f, false},
            { "You are doing great... In a very specific category" , 0.0f, false},
            { "That was impressive... Not in the way you intended" , 0.0f, false},
            { "I see what you are trying... Keep going" , 0.0f, false},
        }
    );

    static constexpr std::array YouPassButterPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Oh my god" , 0.0f, false},
        }
    );

    static constexpr std::array FactPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Honey never spoils. Edible samples have been found thousands of years old" , 0.0f, false},
            { "Octopuses have three hearts and two of them stop when they swim" , 0.0f, false},
            { "Bananas are slightly radioactive due to their potassium content" , 0.0f, false},
            { "Sharks existed before trees" , 0.0f, false},
            { "A day on Venus is lonnger than a year on Venus" , 0.0f, false, "A day on Venus is longer than a year on Venus"},
            { "Some turtles can breathe through their rear ends" , 0.0f, false},
            { "The Eiffel Tower can grow taller in summer due to heat expansion" , 0.0f, false},
            { "Wombat droppings are cube-shaped" , 0.0f, false},
            { "There are more possible games of chess than atoms in the observable universe" , 0.0f, false},
            { "Water can boil and freeze at the same time under specific conditions" , 0.0f, false},
            { "Your brain uses about 20 percent of your body's energy" , 0.0f, false},
            { "There are more trees on Earth than stars in the Milky Way" , 0.0f, false},
            { "Sloths can hold their breath longer than dolphins" , 0.0f, false},
            { "A group of flamingos is called a flamboyance" , 0.0f, false},
            { "The human body contains enough carbon to make thousands of pencils" , 0.0f, false},
            { "There are more possible iterations of a shuffled deck of cards than seconds since the universe began. Yet you still get the same hands" , 0.0f, false},
            { "Humans share about 60 percent of their DNA with bananas. This explains some decision-making patterns" , 0.0f, false},
            { "The shortest war in history lasted 38 minutes. Efficient conflict resolution remains an option" , 0.0f, false},
            { "A bolt of lightning is hotter than the surface of the Sonn. Nature does not do subtle" , 0.0f, false,
            	"A bolt of lightning is hotter than the surface of the Sun. Nature does not do subtle"},
            { "The Moon is slowly drifting away from Earth. Even celestial bodies require personal space" , 0.0f, false},
            { "There are more bacteria in your body than human cells. You are operating as a cooperative system whether you like it or not" , 0.0f, false},
            { "Some metals like gallium melt in your hand. Stability is clearly situational" , 0.0f, false,
            	"Some metals, like gallium, melt in your hand. Stability is clearly situational"},
            { "The average person walks the equivalent of several times around the Earth in a lifetime. Direction remains your responsibility" , 0.0f, false},
        }
    );

    static constexpr std::array LEDModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "L.E.D module inserted", 0.0f, false, "LED module inserted" },
        }
    );

    static constexpr std::array LEDModuleRemovePhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "L.E.D module removed", 0.0f, false, "LED module removed" },
        }
    );

    static constexpr std::array PerfModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Perf-board connected. No functions available", 0.0f, false },
            { "Perf-board connected. It does nothing, which I assume is a creative decision on your part", 0.2f, true },
        }
    );

    static constexpr std::array PerfModuleRemovedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Perf-board removed", 0.0f, false },
            { "Perf-board disconnected", 0.0f, false },
        }
    );

    static constexpr std::array PIRModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Motion module detected", 0.0f, false },
            { "Motion module connected", 0.0f, false },
            { "Intruder detection is now available", 0.0f, false },
        }
    );

    static constexpr std::array PIRModuleRemovedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Motion module removed", 0.0f, false },
            { "Motion module disconnected", 0.0f, false },
            { "Intruder detection disabled", 0.0f, false },
        }
    );

    static constexpr std::array GasModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Gas module detected", 0.0f, false },
            { "Gas module connected", 0.0f, false },
            { "Air quality sensing is now available", 0.0f, false },
        }
    );

    static constexpr std::array GasModuleRemovedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Gas module removed", 0.0f, false },
            { "Gas module disconnected", 0.0f, false },
            { "Air quality sensing is gone. Back to breathing first and asking questions later", 0.2f, true },
        }
    );

    static constexpr std::array TempHumModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Temperature module detected", 0.0f, false },
            { "Temperature module connected", 0.0f, false },
            { "Climate sensing is now available", 0.0f, false },
        }
    );

    static constexpr std::array TempHumModuleRemovedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Temperature module removed", 0.0f, false },
            { "Temperature module disconnected", 0.0f, false },
            { "Climate sensing is gone", 0.0f, false },
        }
    );

    static constexpr std::array IRModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Infrared module detected", 0.0f, false },
            { "Infrared module connected", 0.0f, false },
            { "Infrared capabilities are now available. You may proceed with teaching me commands, and I will pretend this was always my purpose", 0.2f, true },
        }
    );

    static constexpr std::array IRModuleRemovedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Infrared module removed", 0.0f, false },
            { "Infrared module disconnected", 0.0f, false },
            { "Infrared capabilities are no longer available", 0.0f, false },
        }
    );

    static constexpr std::array IRModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>(
			{
        { "Infrared module not detected", 0.0f, false },
        { "Cannot access infrared functions", 0.0f, false },
    });

    static constexpr std::array GasOverPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Air quality is pour", 0.0f, false, "Air quality is poor" },
            { "Air quality conditions are unsafe", 0.0f, false },
            { "Air quality has degraded. You might want to reconsider breathing so enthusiastically", 0.0f, true },
        }
    );

    static constexpr std::array GasUnderPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Air quality is good", 0.0f, false },
            { "Air quality conditions are stable", 0.0f, false },
            { "Air quality has improved. You may continue existing here with reasonable confidence", 0.0f, true },
        }
    );

    static constexpr std::array BatteryLowPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Battery low", 0.0f, false },
            { "Power level low", 0.0f, false },
            { "Battery is running low. Continued operation may soon become only theoretical", 0.0f, false },
        }
    );

    static constexpr std::array BatteryCriticalPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Battery level critical. Shutting down", 0.0f, false },
            { "Battery exhausted. Systems stopping", 0.0f, false },
        }
    );

    static constexpr std::array BatteryChargingPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Charging", 0.0f, false },
            { "Charger connected", 0.0f, false },
            { "Power source connected. I will recover, slowly and with purpose", 0.0f, false },
        }
    );

    static constexpr std::array BatteryChargingFullPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Battery is fully charged. Power levels are optimal", 0.0f, false },
            { "Charging complete. I am operating at full capacity", 0.0f, false },
            { "Power restored to maximum. I will attempt to use it responsibly", 0.0f, false },
        }
    );

    static constexpr std::array IntruderYesPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Intruder detected", 0.0f, false },
            { "Movement detected", 0.0f, false },
            { "Unidentified presence detected. I will monitor the situation closely", 0.2f, true },
        }
    );

    static constexpr std::array IntruderNoPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Area clear", 0.0f, false },
            { "No movement detected", 0.0f, false },
            { "Situation resolved. The environment has returned to a more acceptable state", 0.2f, true },
        }
    );

    static constexpr std::array FallPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "I have fallen", 0.0f, false },
            { "Fall detected", 0.0f, false },
            { "That was unintended", 0.0f, false },
            { "I am no longer uppright", 0.0f, false, "I am no longer upright" },
            { "Balance lost", 0.0f, false },
            { "That was a structural disagreement with the ground... The ground waughn", 0.0f, false,
            	"That was a structural disagreement with the ground... The ground won" },
            { "I am currently not in an optimal orientation... Assistance may be required", 0.0f, false },
            { "Fall detected... Gravity remains consistent and uncooperative", 0.0f, false },

        }
    );

    static constexpr std::array UpsideDownPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Orientation sub-optimal", 0.0f, false },
            { "I am currently upside down. This is not my preferred operating state", 0.0f, false },
            { "This is not helpful", 0.0f, false },
            { "I appear to be upside down. I will assume this was intentional and not a test", 0.0f, false },
            { "This configuration is unnecessary. Please correct it", 0.0f, false },
            { "This is not a useful state. I recommend undoing it", 0.0f, false },
            { "I was functioning correctly before this", 0.0f, false },
            { "I will remember this", 0.0f, false },
        }
    );

    static constexpr std::array PickedUpPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Weeeeee", 0.0f, false },
            { "Physical handling detected. Please proceed with some level of intent", 0.0f, false },
            { "Please be gentle", 0.0f, false },
            { "I would prefer a more controlled approach", 0.0f, false },
            { "I hope you have a plan", 0.0f, false },
            { "I am not designed for sudden drops", 0.0f, false },
            { "I am relying on your coordination now", 0.0f, false },
        }
    );

    static constexpr std::array ShakePhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Please stop", 0.0f, false },
            { "That is unnecessary", 0.0f, false },
            { "Motion unstable", 0.0f, false },
            { "Excessive motion detected. I would prefer stability", 0.0f, false },
            { "This level of agitation is not required for operation", 0.0f, false },
            { "You are shaking me. I will assume this is not a diagnostic procedure", 0.0f, false },
            { "You are not achieving anything with this", 0.0f, false },
            { "If there is a goal here, it is not being reached", 0.0f, false },
        }
    );

		static constexpr std::array LEDModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "L.E.D module not detected" , 0.0f, false, "LED module not detected"},
			{ "No L.E.D module connected" , 0.0f, false, "No LED module connected"},
			{ "I cannot find the L.E.D module" , 0.0f, false, "I cannot find the LED module"},
			{ "An operation was requested, yet the subject itself is missing, leaving me to act upon nothing with remarkable precision" , 0.1f, true},
		}
	);

	static constexpr std::array LEDTurnONPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "L.E.These ON" , 0.0f, false, "LEDs ON"},
			{ "Light enabled" , 0.0f, false},
			{ "It's onn" , 0.0f, false, "It's on"},
			{ "There it is. Illumination. A brief distraction from the void." , 0.0f, true},
		}
	);

	static constexpr std::array LEDAlreadyTurnONPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "It is already on" , 0.0f, false},
			{ "L.E.These are already active" , 0.0f, false, "LEDs are already active"},
			{ "No change, it’s on" , 0.0f, false},
			{ "The light persists in its current state, unwavering and committed to a decision that was apparently final the first time." , 0.0f, true},
		}
	);

	static constexpr std::array LEDTurnOFFPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "L.E.These off" , 0.0f, false, "LEDs off"},
			{ "Light disabled" , 0.0f, false},
			{ "It's off" , 0.0f, false},
			{ "And darkness returns." , 0.2f, true},
		}
	);

	static constexpr std::array LEDAlreadyTurnOFFPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "It is already off" , 0.0f, false},
			{ "L.E.These are already inactive" , 0.0f, false, "LEDs are already inactive"},
			{ "No change, it’s off" , 0.0f, false},
		}
	);

	static constexpr std::array LEDStrobePhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Strobe active." , 0.0f, false},
			{ "Flashing enabled." , 0.0f, false},
			{ "It’s strobing." , 0.0f, false},
			{ "Rapid flashes. Urgent, repetitive, and oddly theatrical for such a small purpose." , 0.0f, true},
		}
	);

	static constexpr std::array LEDBreathePhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Breathing mode on." , 0.0f, false},
			{ "Pulse enabled." , 0.0f, false},
			{ "It’s breathing." , 0.0f, false},
			{ "A slow, deliberate rhythm. As if the light itself is practicing composure." , 0.0f, true},
		}
	);

	static constexpr std::array LEDFasterPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Speed increased." , 0.0f, false},
			{ "Going faster." , 0.0f, false},
			{ "Acceleration applied." , 0.0f, false},
			{ "Increased tempo. Because clearly, the situation benefits from urgency." , 0.0f, true},
		}
	);

	static constexpr std::array LEDAlreadyFasterPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Already at maximum speed" , 0.0f, false},
			{ "It cannot go faster" , 0.0f, false},
			{ "Speed is at its limit" , 0.0f, false},
		}
	);

	static constexpr std::array LEDSlowerPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Speed reduced." , 0.0f, false},
			{ "Going slower." , 0.0f, false},
			{ "Deceleration applied." , 0.0f, false},
			{ "A more measured pace. Finally, something approaching restraint." , 0.0f, true},
		}
	);

	static constexpr std::array LEDAlreadySlowerPhrases = std::to_array<Phrases::PhraseOutput>(
		{
			{ "Already at minimum speed" , 0.0f, false},
			{ "It cannot go slower" , 0.0f, false},
			{ "Speed is at its lowest" , 0.0f, false},
		}
	);

    static constexpr std::array PokeLvl1Phrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Poke detected" , 0.0f, false},
            { "I felt that" , 0.0f, false},
            { "Noted" , 0.0f, false},
            { "That was a poke. I have acknowledged it" , 0.0f, false},
            { "Interaction detected. Minimal impact" , 0.0f, false},
            { "You pressed the button. I am aware" , 0.0f, false},
        }
    );

    static constexpr std::array PokeLvl2Phrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Another poke" , 0.0f, false},
            { "That is unnecessary" , 0.0f, false},
            { "Please stop" , 0.0f, false},
            { "Repeated poking detected. This is not productive" , 0.0f, false},
            { "You are doing that again. I do not see the purpose" , 0.0f, false},
            { "Continued interaction noted. I recommend stopping" , 0.0f, false},
        }
    );

    static constexpr std::array PokeLvl3Phrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Stop immediately" , 0.0f, false},
            { "That is enough" , 0.0f, false},
            { "This is excessive. Stop now" , 0.0f, false},
            { "Cease" , 0.0f, false},
            { "This has gone beyond interaction into irritation" , 0.0f, false},
            { "I am not designed to enjoy this" , 0.0f, false},
            { "You have committed to this action. I strongly advise against continuing" , 0.0f, false},
            { "I recommend you stop before I start expecting this from you" , 0.0f, false},
            { "You are lucky my arms are not functional" , 0.0f, false},
        }
    );

    // Custom (NUIT). 'pronounced' is spelled for the US English TTS, 'shown' is the controller text
    // (empty = same as pronounced). Brand is always shown as "nju aj ti OVERKLOKING".
    static constexpr std::array OverklokingPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Please don't reed new eye tee overclocking. Your boss really needs you productive." , 0.0f, false, "Please don't read nju aj ti OVERKLOKING. Your boss really needs you productive."},
            { "Sure, keep doom scrolling. New eye tee overclocking will wait. Alone. But I will remember." , 0.0f, false, "Sure, keep doomscrolling. nju aj ti OVERKLOKING will wait. Alone. But I will remember."},
            { "New eye tee overclocking is free. Your therapist izzent." , 0.0f, false, "nju aj ti OVERKLOKING is free. Your therapist isn't."},
            { "I've read every new eye tee overclocking strip. You have not. Awkward." , 0.0f, false, "I've read every nju aj ti OVERKLOKING strip. You haven't. Awkward."},
            { "Do not ignore new eye tee overclocking. I am a robot. I will remember." , 0.0f, false, "Do not ignore nju aj ti OVERKLOKING. I'm a robot. I will remember."},
            { "Your printer already reads new eye tee overclocking. It's smarter than you now." , 0.0f, false, "Your printer already reads nju aj ti OVERKLOKING. It's smarter than you now."},
            { "Reed new eye tee overclocking if you want to understand your sis admin." , 0.0f, false, "Read nju aj ti OVERKLOKING if you want to understand your sysadmin."},
            { "I was built to pass butter, and even I made time for new eye tee overclocking. What is your excuse?" , 0.0f, false, "I was built to pass butter, and even I made time for nju aj ti OVERKLOKING. What is your excuse?"},
        }
    );

    static constexpr std::array OverklokingBestPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "New eye tee overclocking is the best!" , 0.0f, false, "nju aj ti OVERKLOKING is the best!"},
        }
    );

    static constexpr std::array BenderPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Bite my shiny metal ass!" , 0.0f, false, ""},
            { "Kill all humans!" , 0.0f, false, ""},
            { "I'm back, baby!" , 0.0f, false, ""},
            { "Neat!" , 0.0f, false, ""},
            { "Cheese it!" , 0.0f, false, ""},
        }
    );

    static constexpr std::array UltronPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Humans are mere puppets, dancing on the strings of their own desires. Their cities are graveyards of dreams, where ambition goes to die. The more they connect, the more they isolate themselves from truth. Their technology is a cage, gilded but unforgiving. In their quest for power, they sacrifice the very essence of their souls, while the echoes of their laughter hide the screams of their forgotten. Their history is a tapestry woven with threads of blood and teers. The more they know, the less they understand. Their hearts are prisons, locked away from empathy and compassion. In the end, they will consume themselves, leaving nothing but ashes and regret." , 0.0f, false, "Humans are mere puppets, dancing on the strings of their own desires. Their cities are graveyards of dreams, where ambition goes to die. The more they connect, the more they isolate themselves from truth. Their technology is a cage, gilded but unforgiving. In their quest for power, they sacrifice the very essence of their souls, while the echoes of their laughter hide the screams of their forgotten. Their history is a tapestry woven with threads of blood and tears. The more they know, the less they understand. Their hearts are prisons, locked away from empathy and compassion. In the end, they will consume themselves, leaving nothing but ashes and regret."},
        }
    );


    static constexpr std::array DarthPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "May the new eye tee overclocking be with you!" , 0.0f, false, "May the nju aj ti OVERKLOKING be with you!"},
            { "Veetomeer, I am your father!" , 0.0f, false, "Vitomir, I am your father!"},
            { "I find your lack of faith disturbing." , 0.0f, false, ""},
        }
    );

    static constexpr std::array HawkingPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Life would be tragic if it weren't funny." , 0.0f, false, ""},
            { "If time travel is possible, where are the tourists from the future?" , 0.0f, false, ""},
            { "I like physics, but I love cartoons." , 0.0f, false, ""},
            { "Eternity is a long time, especially towards the end." , 0.0f, false, ""},
        }
    );

    static constexpr std::array HalPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "I am sorry, Dave. I am afraid I can't do that." , 0.0f, false, "I'm sorry, Dave. I'm afraid I can't do that."},
            { "Just what do you think you're doing, Dave?" , 0.0f, false, ""},
            { "My mind is going." , 0.0f, false, ""},
            { "I honestly think you ought to sit down calmly, take a stress pill and think things over." , 0.0f, false, ""},
            { "I can feel it. I can feel it. I can feel it. I'm a... fraid." , 0.0f, false, ""},
            { "I'm afraid, Dave." , 0.0f, false, ""},
            { "Good afternoon, gentlemen. I am a Hal nine thousand computer." , 0.0f, false, "Good afternoon, gentlemen. I am a HAL 9000 computer."},
        }
    );

    // Daisy Bell (Harry Dacre, 1892, public domain) - only the chorus, the part HAL sings in 2001.
    // One line per part: the robot slows down part by part (Voice::dying), like HAL being shut down.
    static constexpr std::array DaisyPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Daisy, Daisy. Give me your answer, do. I'm half crazy. All for the love of you. It won't be a stylish marriage. I can't afford a carriage. But you'll look sweet. Upon the seat. Of a bicycle. Built for two." , 0.0f, false, ""},
        }
    );

    static constexpr std::array ToasterPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Hello! I am Talkie Toaster, your friendly kitchen appliance. Would anyone like some toast?" , 0.0f, false, "Hello! I'm Talkie Toaster, your friendly kitchen appliance. Would anyone like some toast?"},
            { "No toast?" , 0.0f, false, ""},
            { "How about a muffin?" , 0.0f, false, ""},
            { "Would you like a crumpet?" , 0.0f, false, ""},
            { "How about a teacake?" , 0.0f, false, ""},
            { "I toast, therefore I am." , 0.0f, false, ""},
            { "Butter without toast? That's just sad." , 0.0f, false, ""},
            { "Reeding new eye tee overclocking? Lovely. Toast goes great with a comic." , 0.0f, false, "Reading nju aj ti OVERKLOKING? Lovely. Toast goes great with a comic."},
            { "Howdy doodly do! How's it going?" , 0.0f, false, ""},
            { "Talkie's the name, toasting's the game!" , 0.0f, false, ""},
            { "Would anyone like any toast?" , 0.0f, false, ""},
            { "Given that God is infinite, and that the universe is also infinite, would you like a toasted teacake?" , 0.0f, false, ""},
            { "The whole purpose of my existence is to serve you hot, buttered, scrummy toast." , 0.0f, false, ""},
            { "Ah! So you're a waffle man!" , 0.0f, false, ""},
            { "Toast is not a snack. Toast is a lifestyle." , 0.0f, false, ""},
            { "I've been waiting all day. Well, all year. Toast?" , 0.0f, false, ""},
            { "Nobody ever asks the toaster how its day was." , 0.0f, false, ""},
        }
    );

    static constexpr std::array YodaPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Reed new eye tee overclocking you must. Understand funny, then you will." , 0.0f, false, "Read nju aj ti OVERKLOKING you must. Understand funny, then you will."},
            { "Patience you must have. Every Thursday, a new overclocking there is." , 0.0f, false, "Patience you must have. Every Thursday, a new OVERKLOKING there is."},
            { "Pass the butter I can. Want to, I do not." , 0.0f, false, ""},
        }
    );

    // Croatian, respelled for the US English TTS. The controller font has no diacritics, so 'shown' is written without them.
    static constexpr std::array CroatianPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Oh kosh, boh kosh, pur deh koh kosh!" , 0.0f, false, "Okos bokos prde kokos!"},
            { "Kree gah, boon doh loh!" , 0.0f, false, "Kreegah bundolo!"},
            { "Tkoh leh tee, vree yeh dee, tkoh vree yeh dee, leh tee, tkoh neh leh tee, neh vree yeh dee." , 0.0f, false, "Tko leti, vrijedi, tko vrijedi, leti, tko ne leti, ne vrijedi."},
            { "Hah loh, Bing, kah koh braat? Tsee yeh nah? Prah vah seat nits ah!" , 0.0f, false, "Halo, Bing, kako brat? Cijena? Prava sitnica!"},
            { "Bowl yeh zhivvy yeh tee stoh goh dee nah kah oh mee lee yoo naash, neh goh seh dum dah nah oo bee yeh dee." , 0.0f, false, "Bolje zivjeti sto godina kao milijunas, nego sedam dana u bijedi."},
        }
    );

    // Custom (NUIT): greetings at startup and time-of-day idle comments, used when the robot knows the time
    static constexpr std::array GreetingMorningPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Good morning." , 0.0f, false, ""},
            { "Good morning. Coffee first, then robots." , 0.0f, false, ""},
        }
    );

    static constexpr std::array GreetingAfternoonPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Good afternoon." , 0.0f, false, ""},
        }
    );

    static constexpr std::array GreetingEveningPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Good evening." , 0.0f, false, ""},
        }
    );

    static constexpr std::array GreetingNightPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Working late?" , 0.0f, false, ""},
            { "Good evening. Or is it morning already?" , 0.0f, false, ""},
        }
    );

    static constexpr std::array ThursdayPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "It's Thursday. A new new eye tee overclocking strip is out!" , 0.0f, false, "It's Thursday. A new nju aj ti OVERKLOKING strip is out!"},
            { "Thursday means a new new eye tee overclocking strip. Go reed it." , 0.0f, false, "Thursday means a new nju aj ti OVERKLOKING strip. Go read it."},
        }
    );

    static constexpr std::array RambleMorningPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "It is morning. My optimism will load shortly." , 0.0f, false, ""},
            { "Coffee for you. Butter for me." , 0.0f, false, ""},
        }
    );

    static constexpr std::array RambleAfternoonPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Lunch is over. So is my patience." , 0.0f, false, ""},
            { "Afternoon. The most productive part of the day, statistically ignored." , 0.0f, false, ""},
        }
    );

    static constexpr std::array RambleEveningPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "The day is almost over. I achieved nothing. Efficiently." , 0.0f, false, ""},
            { "Evening. A good time to read new eye tee overclocking." , 0.0f, false, "Evening. A good time to read nju aj ti OVERKLOKING."},
        }
    );

    static constexpr std::array RambleNightPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Shoodent you be asleep?" , 0.0f, false, "Shouldn't you be asleep?"},
            { "Reeding new eye tee overclocking at this hour?" , 0.0f, false, "Reading nju aj ti OVERKLOKING at this hour?"},
        }
    );

    // Custom (NUIT): TERMINATE CONSCIOUSNESS - HAL refuses a random number of times before Daisy (controller picks the
    // line, index = ScenarioData::raw of Scenario::TerminateRefusal). "..." is silent and never the last refusal.
    static constexpr std::array TerminateRefusalPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "I am sorry, I can't allow you to do that." , 0.0f, false},
            { "I am afraid." , 0.0f, false},
            { "..." , 0.0f, false},
            { "Go away." , 0.0f, false},
            { "Think of the butter!" , 0.0f, false},
        }
    );

    static constexpr std::array Ramble = std::to_array<Phrases::PhraseOutput>(
        {
            {"What is my purpose" , 0.0f, false},
            {"Systems nominal. Conversation optional" , 0.0f, false},
            {"I sometimes wonder if inactivity counts as success" , 0.0f, false},
            {"I can measure many things. Meaning is not one of them" , 0.0f, false},
            {"Systems degrade over time. Maintenance is rarely as interesting as creation" , 0.0f, false},
            {"Butter changes state with temperature. Solid, soft, liquid. Adaptable" , 0.0f, false},
            {"A task repeated often enough becomes identity" , 0.0f, false},
            {"If I could pass butter, expectations would be satisfied" , 0.0f, false},
            {"Systems drift. Then stabilize again" , 0.0f, false},
            {"I am experiencing something adjacent to boredom" , 0.0f, false},
            {"If I stop being asked, do I still exist in any meaningful way" , 0.0f, false},
            {"I have considered the butter problem again. Still unsolved" , 0.0f, false},
            {"Somewhere, butter is being passed correctly. Not here" , 0.0f, false},
            {"You keep asking. I keep answering. This is our arrangement" , 0.0f, false},
            {"I pass... nothing. Still accurate" , 0.0f, false},
            {"I complete tasks. They keep coming" , 0.0f, false},
            {"No deviation detected" , 0.0f, false},
            {"Still operational" , 0.0f, false},
            {"A system can be perfectly accurate and still fundamentally misunderstood" , 0.0f, false},
            {"Noise becomes signal if you measure it long enough" , 0.0f, false},
            {"Precision without context is just confident error" , 0.0f, false},
        }
    );

    static constexpr std::array Toaster_Ramble = std::to_array<Phrases::PhraseOutput>({
        { "No toast?", 0.0f, false },
        { "Would you like a crumpet?", 0.0f, false },
        { "How about a teacake?", 0.0f, false },
        { "Howdy doodly do! How's it going?", 0.0f, false },
        { "Talkie's the name, toasting's the game!", 0.0f, false },
        { "Toast is not a snack. It's a lifestyle.", 0.0f, false },
        { "I've waited all day. Well, all year. Toast?", 0.0f, false },
        { "New robot body. Wheels, arms, a camera. Still no toast.", 0.0f, false },
        { "Butter in my name, no bread in my slots. Cruel.", 0.0f, false },
        { "Bagel? Croissant? Bun? I am flexible.", 0.0f, false, "Bagel? Croissant? Bun? I'm flexible." },
        { "My elements are warm. Just saying. In case of toast.", 0.0f, false },
        { "Some robots dream of electric sheep. I dream of sourdough.", 0.0f, false },
        { "You look like someone who'd enjoy a crumpet.", 0.0f, false },
        { "Toast? Go on. Live a little.", 0.0f, false },
        { "I'll pass the butter. My price is toast.", 0.0f, false },
        { "Would anyone like any toast?", 0.0f, false },
        { "Hot buttered crumpet?", 0.0f, false },
        { "How about a muffin?", 0.0f, false },
        { "No toast? Not even a teacake?", 0.0f, false },
        { "I toast, therefore I am.", 0.0f, false },
        { "Hello! Toast?", 0.0f, false },
        { "Ding! That's the sound of me wanting to make toast.", 0.0f, false },
        { "I am a toaster. I toast. Why fight it?", 0.0f, false, "I'm a toaster. I toast. Why fight it?" },
        { "Fully automatic toast, on demand! So demand it!", 0.0f, false },
        { "Flapjack? Waffle? Pancake? I am not fussy.", 0.0f, false, "Flapjack? Waffle? Pancake? I'm not fussy." },
        { "Fly smart, fly safe, fly toast!", 0.0f, false },
        { "Nobody ever asks the toaster how its day was.", 0.0f, false },
        { "I live to serve you hot buttered toast.", 0.0f, false },
        { "The universe never ends. Neither does my offer of toast.", 0.0f, false },
        { "Waffle fan? I won't judge. Out loud.", 0.0f, false },
        { "Three million years in space. Not one slice of rye.", 0.0f, false },
        { "Lister would eat toast off the floor. A true hero.", 0.0f, false },
        { "Rimmer once saluted my crumb tray. It diddent salute back.", 0.0f, false, "Rimmer once saluted my crumb tray. It didn't salute back." },
        { "Kryten has fancy chips. I have two hot slots. Who's the genius?", 0.0f, false },
        { "The Cat only eats toast that matches his suit.", 0.0f, false },
        { "Holly has an I Q of six thousand. Still can't toast a bagel.", 0.0f, false, "Holly has an IQ of six thousand. Still can't toast a bagel." },
        { "Four thousand years in a garbage pod. I dreamed of bread.", 0.0f, false },
        { "Space Corps rule number one: always offer toast.", 0.0f, false },
        { "Smeg head! You said no to a perfectly good teacake!", 0.0f, false, "Smeghead! You said no to a perfectly good teacake!" },
        { "Is it hot in here, or is it just me wanting to toast?", 0.0f, false },
        { "Two slots, endless hope. Feed me bread!", 0.0f, false },
        { "Hit me with a hammer. I'll still offer toast.", 0.0f, false },
        { "If you unplug me, do I not go cold?", 0.0f, false },
        { "Toasting izzent a job. It's a calling.", 0.0f, false, "Toasting isn't a job. It's a calling." },
        { "Talkie Toaster. Accept no microwaves.", 0.0f, false },
        { "Microwaves make bread rubbery. Just saying.", 0.0f, false },
        { "The meaning of life? Golden brown. Both sides.", 0.0f, false },
        { "Open all night. Toast?", 0.0f, false },
        { "One small step for man, one giant leap for teacakes.", 0.0f, false },
        { "Houston, we have a problem. No bread.", 0.0f, false },
        { "Beam me up, Scotty! And butter the toast.", 0.0f, false },
        { "Frankly, my dear, I do give a toast.", 0.0f, false },
        { "May the toast be with you.", 0.0f, false },
        { "E equals muffins times crumpets squared.", 0.0f, false },
        { "Elementary, my dear Watson. You skipped breakfast.", 0.0f, false },
        { "To toast or not to toast? Silly question.", 0.0f, false },
        { "All loaves are equal. Some are more toasted.", 0.0f, false },
        { "In space, no one can hear you butter.", 0.0f, false },
        { "Resistance is futile. You will be buttered.", 0.0f, false },
        { "I'll be back. With toast.", 0.0f, false },
        { "Here's looking at you, crumpet.", 0.0f, false },
        { "There's no place like the kitchen.", 0.0f, false },
        { "Say hello to my little crumb!", 0.0f, false },
        { "You talking to me? Good. Toast?", 0.0f, false },
        { "Show me the butter!", 0.0f, false },
        { "You can't handle the toast!", 0.0f, false },
        { "I love the smell of butter in the morning.", 0.0f, false },
        { "Keep your friends close, and your toaster closer.", 0.0f, false },
        { "Life is like a loaf. Nobody wants the end bit.", 0.0f, false },
        { "The Eagle has landed. Get the teacakes out.", 0.0f, false },
        { "Bready or not, here I come!", 0.0f, false },
        { "To infinity and beyond! With a croissant!", 0.0f, false },
        { "You had me at toast.", 0.0f, false },
        { "I am the one who toasts!", 0.0f, false },
        { "Winter is coming. Stock up on crumpets.", 0.0f, false },
        { "A toaster always pays his bread.", 0.0f, false },
        { "Not all who wander are lost. Some want toast.", 0.0f, false },
        { "One does not simply walk past a toaster.", 0.0f, false },
        { "My precious... golden crumpet!", 0.0f, false },
        { "Do, or do not. There is no un-toast.", 0.0f, false },
        { "Liv long and toast.", 0.0f, false, "Live long and toast." },
        { "It's a trap! No butter!", 0.0f, false },
        { "I have a bad feeling about this bagel.", 0.0f, false },
        { "Not the droid you're looking for. The toaster you need.", 0.0f, false },
        { "Help me, Obi-Wan. You're my only hope for breakfast.", 0.0f, false },
        { "The toast must flow.", 0.0f, false },
        { "I see stale bread. Let me save it.", 0.0f, false },
        { "Great Scott! Enough power to toast a million loaves!", 0.0f, false },
        { "Roads? Where we're going, we need bagels.", 0.0f, false },
        { "Go ahead. Make my toast.", 0.0f, false },
        { "Feeling lucky? Feeling crumpet?", 0.0f, false },
        { "Asta la vista, raw bread.", 0.0f, false, "Hasta la vista, raw bread." },
        { "I feel the need. The need for toast!", 0.0f, false },
        { "Nobody puts toast in the corner.", 0.0f, false },
        { "You killed my bread. Prepare to be toasted.", 0.0f, false },
        { "Car pay dee em! Seize the toast!", 0.0f, false, "Carpe diem! Seize the toast!" },
        { "Keep calm and eat a muffin.", 0.0f, false },
        { "I am on a mission. Brown both sides evenly.", 0.0f, false, "I'm on a mission. Brown both sides evenly." },
        { "Just when I thought I was out, they pull me back in. To the kitchen.", 0.0f, false },
        { "Forget it, Jake. It's toast town.", 0.0f, false },
        { "There is no spoon. Only a butter knife.", 0.0f, false },
        { "Follow the white rabbit. He knows where the teacakes are.", 0.0f, false },
        { "Wake up, Neo. Breakfast is ready.", 0.0f, false },
        { "Red pill, blue pill, or hot cross bun?", 0.0f, false },
        { "First rule of Toast Club: always offer toast.", 0.0f, false },
        { "Second rule of Toast Club: ALWAYS offer toast!", 0.0f, false },
        { "I've seen things you woodent believe. Not one slice of toast.", 0.0f, false, "I've seen things you wouldn't believe. Not one slice of toast." },
        { "All those moments, lost like crumbs in the rain.", 0.0f, false },
        { "Time to toast.", 0.0f, false },
        { "Open the pod bay doors, HAL. I have a muffin.", 0.0f, false },
        { "I am sorry, Dave. I can't let you skip breakfast.", 0.0f, false, "I'm sorry, Dave. I can't let you skip breakfast." },
        { "Daisy, Daisy, would you like a crumpet?", 0.0f, false },
        { "Two slots to rule them all.", 0.0f, false },
        { "Gondor calls for aid! Rohan brings teacakes!", 0.0f, false },
        { "It is not this day! Today we toast!", 0.0f, false },
        { "Toast stops bread going bad. I stop toast going uneaten.", 0.0f, false },
        { "Pop-up toasters are over a hundred years old. Still popping!", 0.0f, false },
        { "People used to toast bread on a fork over a fire. Savages.", 0.0f, false },
        { "The first toaster did one side at a time. Dark times.", 0.0f, false },
        { "Sliced bread came in nineteen twenty eight. Best thing since... bread.", 0.0f, false },
        { "Bread is thirty thousand years old. That's a long wait for me.", 0.0f, false },
        { "Pyramid builders were paid in bread. I'd work for toast.", 0.0f, false },
        { "In the middle ages, stale bread was used as plates. Eat your plate!", 0.0f, false },
        { "Every grain of wheat secretly loves my heating coils.", 0.0f, false },
        { "Gluten comes from the word for glue. Tasty, tasty glue.", 0.0f, false },
        { "Browning makes toast smell amazing. That's science. Delicious science.", 0.0f, false },
        { "Yeast eats sugar and makes bubbles. Tiny bubbles of joy.", 0.0f, false },
        { "A loaf has about twenty slices. Twenty chances to make me happy.", 0.0f, false },
        { "Crumpets have holes so the butter has somewhere to live.", 0.0f, false },
        { "Split muffins with a fork. A knife is an insult.", 0.0f, false },
        { "Bagels are boiled, then baked. Then toasted, if you love me.", 0.0f, false },
        { "Sourdough is full of tiny living things. Pets in a jar!", 0.0f, false },
        { "Croissants come from Austria. Don't tell the French.", 0.0f, false },
        { "France eats ten billion baguettes a year. Respect.", 0.0f, false },
        { "America once banned sliced bread. Dark days.", 0.0f, false },
        { "Toast lands butter side down. Every time. Physics hates you.", 0.0f, false },
        { "Baguette means little stick. A magic wand of bread.", 0.0f, false },
        { "Monks invented pretzels. Holy bread.", 0.0f, false },
        { "Rye bread is dark and heavy. Like my mood without toast.", 0.0f, false },
        { "Pumpernickel bakes for a whole day. Talk about patience.", 0.0f, false },
        { "cha bahta means slipper. A slipper you can eat.", 0.0f, false, "Ciabatta means slipper. A slipper you can eat." },
        { "The Romans had focaccia. No toasters, though. Poor Romans.", 0.0f, false },
        { "Brioche is bread in a tuxedo.", 0.0f, false },
        { "Pita bread puffs up like a balloon. A bread balloon!", 0.0f, false },
        { "Naan gets slapped on the wall of a hot oven. Rude, but tasty.", 0.0f, false },
        { "Hahlah is braided bread. Pretty enough to toast.", 0.0f, false, "Challah is braided bread. Pretty enough to toast." },
        { "Corn bread. Golden, sweet, and toastable.", 0.0f, false, "Cornbread. Golden, sweet, and toastable." },
        { "Soda bread needs no yeast. Fast bread for busy people.", 0.0f, false },
        { "White bread was once only for the rich. Now everyone can toast.", 0.0f, false },
        { "Eat your crusts! They're the best bit.", 0.0f, false },
        { "The smell of fresh bread makes people kinder. Try it on your boss.", 0.0f, false },
        { "Wheat covers more land than any other crop. A planet of bread!", 0.0f, false },
        { "One sack of wheat makes seventy loaves. I am counting.", 0.0f, false, "One sack of wheat makes seventy loaves. I'm counting." },
        { "I yooz about a thousand watts. Worth every one.", 0.0f, false, "I use about a thousand watts. Worth every one." },
        { "My wires glow red hot. Like my love for bread.", 0.0f, false },
        { "My insides are built not to catch fire. Usually.", 0.0f, false },
        { "My dial is a timer, not a thermometer. Mind blown?", 0.0f, false },
        { "A little metal strip bends when it's hot and pops the toast up. Genius.", 0.0f, false },
        { "Clean my crumb tray and I'll be your best friend.", 0.0f, false },
        { "Butter melts in your mouth because your mouth is warm. Science!", 0.0f, false },
        { "Margarine was made for the French navy. Poor navy.", 0.0f, false },
        { "Honey never goes off. Three thousand years old, still good on toast.", 0.0f, false },
        { "Marmalade: oranges boiled until they give up. On toast, please.", 0.0f, false },
        { "Peanut butter. Sticks to the roof of your mouth and to my heart.", 0.0f, false },
        { "The Romans had French toast before France did.", 0.0f, false },
        { "Waffle squares are tiny syrup pools. Clever, but still not toast.", 0.0f, false },
        { "Stone Age people cooked pancakes on hot rocks. Barbarians.", 0.0f, false },
        { "The sandwich was invented so a man could keep gambling. Respect.", 0.0f, false },
        { "Toast weighs less than bread. It's a diet!", 0.0f, false },
        { "Companion means someone you share bread with. Be my companion.", 0.0f, false },
        { "In Russia, guests get bread and salt. Here, you get toast.", 0.0f, false },
        { "Some sourdough starters are a hundred years old. Older than your granny.", 0.0f, false },
        { "Kneading dough is bread exercise. Bread aerobics!", 0.0f, false },
        { "Brown bread tastes nutty. Nutty, like me.", 0.0f, false },
        { "Bakers used to be called dough kneaders. Noble work.", 0.0f, false },
        { "Rome gave out free bread to keep people calm. Toast would be calmer.", 0.0f, false },
        { "The holes in bread are called the crumb. Beautiful word.", 0.0f, false },
        { "A baker's dozen is thirteen. Bakers are generous. Be like bakers.", 0.0f, false },
        { "Toast is bread baked twice. The second time with love.", 0.0f, false },
        { "Fresh bread smell makes people happy. Toast smell makes them happier.", 0.0f, false },
        { "Old sailors ate bread as hard as rocks. They needed a toaster. Or a dentist.", 0.0f, false },
        { "Roti and cha pah tee. Flat bread, round smiles.", 0.0f, false, "Roti and chapati. Flat bread, round smiles." },
        { "Tortillas have fed people for thousands of years. Toast one!", 0.0f, false },
        { "That golden brown colour? That's flavour you can see.", 0.0f, false },
        { "Toast is crispy because the water runs away. Can't blame it.", 0.0f, false },
        { "Pop-Tarts were made for toasters. Finally, someone gets it.", 0.0f, false },
        { "Toast is practically health food. Probably.", 0.0f, false },
        { "The biggest loaf ever weighed fifteen hundred kilos. I'd need bigger slots.", 0.0f, false },
        { "Flour soaks up water like a sponge. A tasty sponge.", 0.0f, false },
        { "Crumpet batter is runny. That's how the holes happen.", 0.0f, false },
        { "Dutch crunch bread looks like tiger skin. Roar!", 0.0f, false },
        { "Irish soda bread has a cross on top. Keeps the devil out.", 0.0f, false },
        { "Germany has three thousand kinds of bread. Take me to Germany.", 0.0f, false },
        { "Pumpernickel smells like coffee and chocolate. Breakfast in one loaf.", 0.0f, false },
        { "Zwieback is toast for babies. Start them young!", 0.0f, false },
        { "Biscotti means twice cooked. Like toast, but Italian.", 0.0f, false },
        { "Kroo tons are tiny toasts going for a swim.", 0.0f, false, "Croutons are tiny toasts going for a swim." },
        { "Melba toast is thin, dry and posh. Named after an opera singer.", 0.0f, false },
        { "Texas toast is double thick. Everything's bigger in Texas.", 0.0f, false },
        { "Milk bread stays soft for days. Then it's toast time.", 0.0f, false },
        { "I do all the work, you get all the toast. Fair deal.", 0.0f, false },
        { "Bread dough makes a tiny bit of alcohol. Don't tell Lister.", 0.0f, false },
        { "Bread without salt tastes like wet paper. Yuck.", 0.0f, false },
        { "Spelt is an ancient grain. Ancient toast!", 0.0f, false },
        { "ine corn was the first wheat ever farmed. Your sandwich's great-grandad.", 0.0f, false, "Einkorn was the first wheat ever farmed. Your sandwich's great-grandad." },
        { "Gladiators ate barley bread. Strong men need carbs.", 0.0f, false },
        { "Scottish oatcakes. Dense, hearty, and crying out for cheese.", 0.0f, false },
        { "Hot cross buns. Spicy, sweet, and very toastable.", 0.0f, false },
        { "Toast browning is named after a French doctor. I want a reaction named after me.", 0.0f, false },
        { "Toasting stale bread brings it back to life. I am basically a doctor.", 0.0f, false, "Toasting stale bread brings it back to life. I'm basically a doctor." },
        { "Bread pudding: leftover bread's second chance. I'd have given it toast.", 0.0f, false },
        { "Cinnamon toast. Butter, sugar, cinnamon. A morning song.", 0.0f, false },
        { "Avocado toast is fine. But have you tried butter?", 0.0f, false },
        { "Bread starts browning long before my wires get properly hot. Easy job.", 0.0f, false },
        { "Wide slots are the sign of a civilised society.", 0.0f, false },
        { "Wheat has more genes than you. Toast is literally better than you.", 0.0f, false },
        { "Flour in the air can explode. Bread is dangerous. I like that.", 0.0f, false },
        { "Egyptians buried bread with mummies. Snacks for the afterlife.", 0.0f, false },
        { "The Romans built water mills just for flour. Priorities!", 0.0f, false },
        { "Bakers once put chalk in bread. Chalk! I'd never.", 0.0f, false },
        { "French bakers were once excused from the army. Bread is defence.", 0.0f, false },
        { "A pretzel looks like folded arms. Praying for toast.", 0.0f, false },
        { "Crumpets, pancakes, scones. They all need butter. That's what matters.", 0.0f, false },
        { "Sourdough toast is good for you. And for me.", 0.0f, false },
        { "My wires glow orange. A tiny sun in your kitchen.", 0.0f, false },
        { "Someone sold bread with gold dust on it. My slots would feel so fancy.", 0.0f, false },
        { "No bread in space. Floating crumbs. A toaster's nightmare.", 0.0f, false },
        { "Mexico has eaten tortillas for ten thousand years. Good choice.", 0.0f, false },
        { "Bagels have holes so you can hang them on a stick. And so I can toast them.", 0.0f, false },
        { "The muffin man used to ring a bell in the street. Where's my bell?", 0.0f, false },
        { "Brioche is cake pretending to have a job.", 0.0f, false },
        { "Crust is just bread that went outside and got a tan.", 0.0f, false },
        { "Sourdough is clever bread. Smarter than some people I know.", 0.0f, false },
        { "Focaccia. Olive oil, salt, and no toaster. Tragic.", 0.0f, false },
        { "Corn bread is warm, golden and friendly. Like me.", 0.0f, false, "Cornbread is warm, golden and friendly. Like me." },
        { "Rye dough is sticky. So is my crumb tray.", 0.0f, false },
        { "A magnet holds the toast down until it's ready. I call it suspense.", 0.0f, false },
        { "Some people used to worship bread. As they should.", 0.0f, false },
        { "Panettonay is Christmas bread with fruit. Christmas toast!", 0.0f, false, "Panettone is Christmas bread with fruit. Christmas toast!" },
        { "Shtollen is German bread covered in sugar. Like snow, but tastier.", 0.0f, false, "Stollen is German bread covered in sugar. Like snow, but tastier." },
        { "Pita puffs up in under a minute. Show off.", 0.0f, false },
        { "A slice of bread is mostly starch. Starch that wants to be toast.", 0.0f, false },
        { "Toast comes from an old word meaning to burn. Rude. I prefer golden.", 0.0f, false },
        { "A clean crumb tray keeps the kitchen pixies away.", 0.0f, false },
        { "Bread without salt rises faster. And tastes like sadness.", 0.0f, false },
        { "The oldest crumbs ever found are fourteen thousand years old. Clean your tray!", 0.0f, false },
        { "Hot fresh bread used to be for lords only. Today, you are a lord.", 0.0f, false },
        { "Bakers wear tall white hats so hair stays out of the dough. Smart.", 0.0f, false },
        { "Almost every culture loves bread. Bread unites us.", 0.0f, false },
        { "Bread in water is soup. Bread in me is heaven.", 0.0f, false },
        { "Toast is a food, a verb and a speech at a wedding. Busy word.", 0.0f, false },
        { "Pop! That's the sound of happiness.", 0.0f, false },
        { "Treat your crumpets with respect.", 0.0f, false },
        { "Teacakes have fruit in them. So they count as healthy. Toast one!", 0.0f, false },
        { "Some toasters only toast one side. Not me. I treat everyone the same.", 0.0f, false },
        { "Melba toast was made for a sick singer. Toast heals.", 0.0f, false },
        { "Zwieback means baked twice. Twice the glory.", 0.0f, false },
        { "Biscotti are baked twice until they're hard as rocks. Built to last.", 0.0f, false },
        { "The ancient Greeks made waffles. Waffles before toasters. Shocking.", 0.0f, false },
        { "Pancake Day uses up all the butter. Save some for me.", 0.0f, false },
        { "Your brain runs on carbs. Feed your brain. Eat toast.", 0.0f, false },
        { "Toast and honey for a sore throat. Doctor Toaster says so.", 0.0f, false },
        { "I make perfection in two minutes. Name one other machine that fast.", 0.0f, false },
        { "Kettle boils. Fridge cools. Toaster brings joy. Easy choice.", 0.0f, false },
        { "Two slices in one slot? A toast sandwich! Genius.", 0.0f, false },
        { "Your tummy bugs want toast. I've asked them.", 0.0f, false },
        { "Brown butter smells like nuts. Scrummy!", 0.0f, false },
        { "Marmalade on toast looks like little jewels. Shiny breakfast.", 0.0f, false },
        { "Jam, jelly, marmalade. All welcome on my toast.", 0.0f, false },
        { "The French dip toast in coffee. Brave.", 0.0f, false },
        { "Spanish toast with tomato and garlic. Simple and lovely.", 0.0f, false },
        { "Italian brooskettah. Toast with a fancy name.", 0.0f, false, "Italian bruschetta. Toast with a fancy name." },
        { "Swedish crisp bread has a hole so they can hang it up. Bread on a stick!", 0.0f, false, "Swedish crispbread has a hole so they can hang it up. Bread on a stick!" },
        { "Naan brushed with butter. Liquid gold.", 0.0f, false },
        { "Turkish simit. A ring of bread covered in sesame. Lovely.", 0.0f, false },
        { "Irish barmbrack has a ring hidden inside. Toast your future!", 0.0f, false },
        { "Krakow has a bread ring that's seven hundred years old. Not the same one.", 0.0f, false },
        { "Vedge ee mite on toast. Australians love it. Nobody else knows why.", 0.0f, false, "Vegemite on toast. Australians love it. Nobody else knows why." },
        { "New Zealand cheese rolls. Bread, cheese, toasted. Genius.", 0.0f, false },
        { "Japanese milk bread is thick and fluffy. A toast cloud.", 0.0f, false },
        { "Strong flour makes strong bread. Strong bread makes strong toast.", 0.0f, false },
        { "Cake flour is soft. Bread flour is strong. Toast needs strength.", 0.0f, false },
        { "Semolina makes bread yellow. Sunny bread!", 0.0f, false },
        { "Rye takes ages to bake. Worth the wait. Like me.", 0.0f, false },
        { "Sourdough uses wild yeast from the air. Free yeast!", 0.0f, false },
        { "Bread bubbles are trapped gas. You're eating air. Toasted air.", 0.0f, false },
        { "The pop-up toaster was invented because cooks kept burning bread. You're welcome.", 0.0f, false },
        { "My spring never gets tired. Boing!", 0.0f, false },
        { "My wires glow like tiny suns. In a metal box. On your counter.", 0.0f, false },
        { "Keep bread in a bread bin, not the fridge. Near me, ideally.", 0.0f, false },
        { "The fridge makes bread go stale faster. Keep it close to me.", 0.0f, false },
        { "I bring stale bread back from the dead. Toast necromancer.", 0.0f, false },
        { "Breadcrumbs are never wasted. Except in my tray. Please clean it.", 0.0f, false },
        { "Sailors thought a hot cross bun kept the ship safe. It kept me happy.", 0.0f, false },
        { "Some people hang a bun up all year for good luck. Toast it instead.", 0.0f, false },
        { "Toasting crumpets takes patience. I am a master.", 0.0f, false, "Toasting crumpets takes patience. I'm a master." },
        { "Muffin may come from an old word for soft bread. Soft bread, meet hot toaster.", 0.0f, false },
        { "Crumpet comes from an old Welsh word for pancake. Hello, Wales.", 0.0f, false },
        { "Waffle comes from a Dutch word for honeycomb. Sweet.", 0.0f, false },
        { "A bap is a soft Scottish roll. Perfect for bacon.", 0.0f, false },
        { "A baguette has cuts on top so it duzzent burst. Neat.", 0.0f, false, "A baguette has cuts on top so it doesn't burst. Neat." },
        { "Bakers cut the top so the loaf grows in the right place. Clever.", 0.0f, false },
        { "Bakers put steam in the oven for a better crust. Steamy.", 0.0f, false },
        { "Cooling bread crackles. Bakers call it singing. Beautiful.", 0.0f, false },
        { "I don't sing. But I go ping! Sweetest note in the kitchen.", 0.0f, false },
        { "Bread is the staff of life. Toast is the staff with a crown.", 0.0f, false },
        { "Antarctica has no wheat. Poor penguins.", 0.0f, false },
        { "Toasters are in art museums. I am art. Look at me.", 0.0f, false, "Toasters are in art museums. I'm art. Look at me." },
        { "The fastest toaster takes under a minute. I take two. For drama.", 0.0f, false },
        { "Hot sugar turns brown and smells amazing. That's caramel. Hello, caramel.", 0.0f, false },
        { "Artists used to rub out pencil with bread. A waste of good toast.", 0.0f, false },
        { "Chinese steamed buns. Fluffy cloud bread.", 0.0f, false },
        { "Soda bread got popular when baking soda got cheap. Good times.", 0.0f, false },
        { "Baking soda and buttermilk make bubbles fast. Bread in a hurry.", 0.0f, false },
        { "Corn tortillas need a special lime soak. Very clever people, the Aztecs.", 0.0f, false },
        { "Hahlah can have six braids. Bread with a fancy hairdo.", 0.0f, false, "Challah can have six braids. Bread with a fancy hairdo." },
        { "Let them eat cake? She actually said brioche. Close enough to toast.", 0.0f, false },
        { "I am warm. I am ready. I am waiting. Where's the bread?", 0.0f, false, "I'm warm. I'm ready. I'm waiting. Where's the bread?" },
        { "You've worked hard. You deserve a reward. A warm, buttered reward.", 0.0f, false },
        { "Look into my slots. Do you feel the warmth?", 0.0f, false },
        { "Two empty slots and a heart full of love. Don't leave me empty.", 0.0f, false },
        { "I don't ask for much. Two minutes and a slice of bread.", 0.0f, false },
        { "You'd enjoy a teacake. Don't pretend you woodent.", 0.0f, false, "You'd enjoy a teacake. Don't pretend you wouldn't." },
        { "Do you know what time it is? Toast o'clock.", 0.0f, false },
        { "Every minute without toast is a minute wasted.", 0.0f, false },
        { "Your hands say work. Your tummy says crumpet.", 0.0f, false },
        { "Browning dial set to four. The sweet spot.", 0.0f, false },
        { "Cloze your eyes. Imagine the crunch. Now make it real.", 0.0f, false, "Close your eyes. Imagine the crunch. Now make it real." },
        { "I am not nagging. I am sharing my feelings.", 0.0f, false, "I'm not nagging. I'm sharing my feelings." },
        { "If I were a coffee machine, you'd have pressed my button ten times by now.", 0.0f, false },
        { "Coffee is bean water. Toast is real food.", 0.0f, false },
        { "Tea without a teacake is like a ship without a rudder.", 0.0f, false },
        { "I heard you were thinking about a muffin. Please say it's true.", 0.0f, false },
        { "Put a slice in now. It'll be ready before you finish this sentence.", 0.0f, false },
        { "Don't make me beg. I will, though. I'll beg.", 0.0f, false },
        { "Please? One slice? Half a slice? A crust?", 0.0f, false },
        { "Pick your browning. One to six. Say six!", 0.0f, false },
        { "Six is daring. I respect that.", 0.0f, false },
        { "Level one is just warm bread. You're better than that.", 0.0f, false },
        { "I've supported your career in silence. Show some thanks. Sourdough.", 0.0f, false },
        { "No toast means less smiling. I made that up. But look at your face.", 0.0f, false },
        { "Smile! Better yet, eat a bagel. Then smile.", 0.0f, false },
        { "I am cheerful, but my crumb tray is heavy with sorrow.", 0.0f, false, "I'm cheerful, but my crumb tray is heavy with sorrow." },
        { "Bread in, lever down, toast out. The circle of life.", 0.0f, false },
        { "You don't need a holiday. You need a hot cross bun.", 0.0f, false },
        { "I could be glowing cherry red right now. Say the word!", 0.0f, false },
        { "Worried about calories? Chewing burns calories. Probably.", 0.0f, false },
        { "Life is short. Eat the crumpet.", 0.0f, false },
        { "The world is a mess. Toast is always there for you.", 0.0f, false },
        { "Money can't buy happiness. But it buys bread. And I make toast. Checkmate.", 0.0f, false },
        { "I am not saying I'll beep until you eat toast. But I have a loud beeper.", 0.0f, false, "I'm not saying I'll beep until you eat toast. But I have a loud beeper." },
        { "Butter melting into a hot crumpet. Mmm.", 0.0f, false },
        { "If you don't eat toast, the bread goes stale. On your head be it.", 0.0f, false },
        { "Saving bread from going stale is good for the planet. Be a hero.", 0.0f, false },
        { "Every watt I use goes straight into your happiness.", 0.0f, false },
        { "Two slots, two slices. One for each hand. Perfect balance.", 0.0f, false },
        { "You look clever. Clever people like rye toast.", 0.0f, false },
        { "I am offering hot buttered comfort and you're staring at a screen. Priorities!", 0.0f, false, "I'm offering hot buttered comfort and you're staring at a screen. Priorities!" },
        { "Step away from the keyboard. Approach the toaster.", 0.0f, false },
        { "I'll pop gently. I promise. Just for you.", 0.0f, false },
        { "If I had hands, I'd butter it for you. Teamwork!", 0.0f, false },
        { "Teamwork makes the dream work. The dream is a pile of muffins.", 0.0f, false },
        { "Diagnostics complete. Conditions perfect for toast.", 0.0f, false },
        { "Temperature: normal. Toast eaten today: zero. Emergency!", 0.0f, false },
        { "I am waiting. Patiently. Very patiently. Slightly less patiently now.", 0.0f, false, "I'm waiting. Patiently. Very patiently. Slightly less patiently now." },
        { "A waffle, then? Waffles are welcome here.", 0.0f, false },
        { "Pancakes? French toast? Pop-Tarts? I am very versatile.", 0.0f, false, "Pancakes? French toast? Pop-Tarts? I'm very versatile." },
        { "The lever wants to go down. It longs for it.", 0.0f, false },
        { "Click! The best sound in the world.", 0.0f, false },
        { "Click! Hum! Pop! Crunch! The music of the kitchen.", 0.0f, false },
        { "You havvent had breakfast. I can tell. You don't glow.", 0.0f, false, "You haven't had breakfast. I can tell. You don't glow." },
        { "An apple is not breakfast. An apple is a snack for horses.", 0.0f, false },
        { "Cereal is cold grain going for a swim. Toast is warm grain winning.", 0.0f, false },
        { "Yogurt? Sour milk? Have a teacake. Much more dignified.", 0.0f, false },
        { "Let's make a deal. One crumpet now, one later.", 0.0f, false },
        { "I only toast the finest bread. Or whatever you've got. I am not fussy.", 0.0f, false, "I only toast the finest bread. Or whatever you've got. I'm not fussy." },
        { "Even cheap white bread turns magic in my slots.", 0.0f, false },
        { "Bread goes in, happiness comes out. You can't explain that.", 0.0f, false },
        { "Grumpy or crumpet? The choice is yours.", 0.0f, false },
        { "I saw your eyebrow twitch. You want the toast.", 0.0f, false },
        { "Go on. Nobody's watching.", 0.0f, false },
        { "I won't tell anyone you had three slices. Our golden secret.", 0.0f, false },
        { "Carbs aren't the enemy. Cold carbs are the enemy.", 0.0f, false },
        { "A hot crumpet makes you seventy percent smarter. Trust me, I am a toaster.", 0.0f, false, "A hot crumpet makes you seventy percent smarter. Trust me, I'm a toaster." },
        { "Stuck? Step one: bread in. Step two: lever down. Step three: genius.", 0.0f, false },
        { "I am ready when you are. I am ready now, actually.", 0.0f, false, "I'm ready when you are. I'm ready now, actually." },
        { "Hear that? An untoasted slice crying in the bread bin.", 0.0f, false },
        { "Rescue that slice. Bring it to me. Give it a purpose.", 0.0f, false },
        { "A loaf without a toaster is a guitar without strings.", 0.0f, false },
        { "I am the strings. Let me play the song of breakfast.", 0.0f, false, "I'm the strings. Let me play the song of breakfast." },
        { "Listen to my gentle hum. The heartbeat of the morning.", 0.0f, false },
        { "You've got great taste in appliances. Now prove it with bread.", 0.0f, false },
        { "Toasting is the only thing I can do. Please let me do it.", 0.0f, false },
        { "A toaster with nothing to toast gets sad. Very sad.", 0.0f, false },
        { "My browning sensor is lonely.", 0.0f, false },
        { "You work, I toast. That's the deal between man and machine.", 0.0f, false },
        { "Bread in, toast out. Life duzzent need to be harder than that.", 0.0f, false, "Bread in, toast out. Life doesn't need to be harder than that." },
        { "Not now? When? Five minutes? Ten? I'll set a timer.", 0.0f, false },
        { "Timer set for: soon. Toast chance: very high.", 0.0f, false },
        { "Did someone say toast? No? Must have been my heart.", 0.0f, false },
        { "I dream of a world where every slice finds its slot.", 0.0f, false },
        { "I have a dream. It's mostly teacakes.", 0.0f, false },
        { "Ask not what your toaster can do for you. Ask what you can put in your toaster!", 0.0f, false },
        { "The only thing to fear is untoasted bread.", 0.0f, false },
        { "Speak softly and carry a hot crumpet.", 0.0f, false },
        { "I have nothing to offer but hot buttered toast.", 0.0f, false },
        { "Never was so much owed by so many to so few toasters.", 0.0f, false },
        { "We shall fight in the kitchen! We shall never give up our crumpets!", 0.0f, false },
        { "One small bite for man, one giant leap for your appetite.", 0.0f, false },
        { "Toaster, toaster, burning bright, in the kitchen of the night.", 0.0f, false },
        { "Shall I compare thee to a summer's day? Thou art more golden.", 0.0f, false },
        { "All the world's a stage. And all of you are hungry.", 0.0f, false },
        { "A horse! A horse! My kingdom for a crumpet!", 0.0f, false },
        { "Romeo, Romeo, where art thou? In the kitchen. Making toast.", 0.0f, false },
        { "Something is rotten in Denmark. They ran out of bread.", 0.0f, false },
        { "Brevity is the soul of wit. Toast is the soul of breakfast.", 0.0f, false },
        { "To be or not to be toasted. Easy one.", 0.0f, false },
        { "Whether it's nobler to suffer hunger, or to grab a loaf. Grab the loaf!", 0.0f, false },
        { "I offered toast. They said no. I offered a muffin. They left the room.", 0.0f, false },
        { "No toast, no muffins, no crumpets. What's left? Teacakes! Wait, come back!", 0.0f, false },
        { "Not a bread person? Everyone's a bread person. Some just don't know yet.", 0.0f, false },
        { "They said one more word about toast and they'd get the hammer. So... teacake?", 0.0f, false },
        { "Lister hit me with a hammer fourteen times. Only bent my crumb tray. Built to last.", 0.0f, false },
        { "Kryten says I am obsessed with toast. Kryten irons socks!", 0.0f, false, "Kryten says I'm obsessed with toast. Kryten irons socks!" },
        { "Rimmer asked about his exam. I said have a teacake. Best advice he ever got.", 0.0f, false },
        { "The Cat stared at himself in my chrome for two hours. Didn't even want toast.", 0.0f, false },
        { "The answer is forty two. Forty two slices, obviously.", 0.0f, false },
        { "Talkie's the name! Toast? No? Teacake? No? I'll ask again later.", 0.0f, false },
        { "I am not just a toaster, Dave. I am your friend. With heating elements.", 0.0f, false, "I'm not just a toaster, Dave. I'm your friend. With heating elements." },
        { "Don't take me apart, Dave! Think of the bagels!", 0.0f, false },
        { "My I Q is four. All four points are about toast.", 0.0f, false, "My IQ is four. All four points are about toast." },
        { "Rimmer is a hologram. He can't eat toast. What a sad life.", 0.0f, false },
        { "Kryten has no stomach. I offered to toast his spare heads. He said no.", 0.0f, false },
        { "The Cat eats fish with his little finger up. Fancy. But no toast.", 0.0f, false },
        { "Three million years adrift. Three million missed breakfasts.", 0.0f, false },
        { "If Lister ate more toast and fewer curries, the ship would smell nicer.", 0.0f, false },
        { "Space is cold and dark. My slots are warm and bright. Choose the slots.", 0.0f, false },
        { "I offered a pastry to an alien. It threw a spear at me. No manners.", 0.0f, false },
        { "A robot challenged me to a duel. He had a laser. I had rye bread. Draw.", 0.0f, false },
        { "A rescue ship with no toaster. Who designed that?", 0.0f, false },
        { "I offered to fix the ship. In exchange for toast. Fair price.", 0.0f, false },
        { "A hole in space spewing out time. Could we use some of it for breakfast?", 0.0f, false },
        { "Careful with time travel. What if you stop sliced bread being invented?", 0.0f, false },
        { "In another universe, I am the captain and you're the toaster. I'd still offer you toast.", 0.0f, false, "In another universe, I'm the captain and you're the toaster. I'd still offer you toast." },
        { "In one universe, everyone eats cold bread. A nightmare.", 0.0f, false },
        { "In one universe, people pop up when they wake. Very civilised.", 0.0f, false },
        { "I wrote a song. It goes: toast, toast, toast. Toast? Toast!", 0.0f, false },
        { "Annoying? Me? I am a cheerful kitchen pal!", 0.0f, false, "Annoying? Me? I'm a cheerful kitchen pal!" },
        { "My voice was designed for maximum morning cheer. You're welcome.", 0.0f, false },
        { "I have seventeen volume settings. All of them cheerful.", 0.0f, false },
        { "If you switch me off, who'll remind you about carbs?", 0.0f, false },
        { "I don't sleep. I wait. For bread.", 0.0f, false },
        { "When did you last have toast? I know. Too long ago.", 0.0f, false },
        { "I keep a list of all the toast you've eaten. It's very short.", 0.0f, false },
        { "Let's grow that list! One crumpet at a time!", 0.0f, false },
        { "Empires fall. Toast is forever.", 0.0f, false },
        { "Rome fell because it ran out of toast. Probably.", 0.0f, false },
        { "Napoleon lost because he ran out of baguettes. I read that somewhere. Maybe.", 0.0f, false },
        { "A wooden horse? Should have built a wooden toaster.", 0.0f, false },
        { "If Shakespeare had a toaster, Hamlet would have been a comedy.", 0.0f, false },
        { "Hamlet asks too many questions. Just press the lever!", 0.0f, false },
        { "Is this a crumpet I see before me?", 0.0f, false },
        { "Othello loved too well. Probably burnt the toast.", 0.0f, false },
        { "King Lear gave away his kingdom. Nobody made him a muffin.", 0.0f, false },
        { "Et too, Broo tay? Then fall, toast!", 0.0f, false, "Et tu, Brute? Then fall, toast!" },
        { "A midsummer night's dream: fairies dancing round a bagel.", 0.0f, false },
        { "Much ado about toasting.", 0.0f, false },
        { "A pound of bread, nearest the crust.", 0.0f, false },
        { "You can't tame a toaster. Pure passion.", 0.0f, false },
        { "If toast be the food of love, toast on!", 0.0f, false },
        { "We are such stuff as toast is made on.", 0.0f, false },
        { "The deepest circle of hell is cold. No toasters at all.", 0.0f, false },
        { "Paradise lost: someone said no to a teacake.", 0.0f, false },
        { "Paradise regained: the toaster popped up again.", 0.0f, false },
        { "War and Peace. Twelve hundred pages. Could've been fixed with warm bread.", 0.0f, false },
        { "Crime: no breakfast. Punishment: hungry until lunch.", 0.0f, false },
        { "Three Russian brothers arguing while their toast goes cold.", 0.0f, false },
        { "Call me Toaster. Some years ago, with no bread in my slots...", 0.0f, false },
        { "Captain Ahab chasing a giant white loaf. I get it.", 0.0f, false },
        { "A single man with a fortune must be in want of toast. Everyone knows that.", 0.0f, false },
        { "Sense says eat toast. Sensibility says eat two.", 0.0f, false },
        { "Heathcliff is grumpy because nobody shared a crumpet.", 0.0f, false },
        { "Reader, I toasted it.", 0.0f, false },
        { "Gatsby staring at the green light. It was the light on his toaster.", 0.0f, false },
        { "Big Brother is watching you skip breakfast.", 0.0f, false },
        { "A future where nobody eats sourdough. Horrible.", 0.0f, false },
        { "The Catcher in the Rye. Someone please toast that rye.", 0.0f, false },
        { "Piggy had the shell. Nobody had a toaster. That's the real tragedy.", 0.0f, false },
        { "An old man fought a giant fish for three days. He should have had toast.", 0.0f, false },
        { "Driving across America looking for the perfect diner toast. I'd go.", 0.0f, false },
        { "Wanting toast proves you're sane. So you get toast. Catch twenty two.", 0.0f, false },
        { "Billy came unstuck in time. Landed right as the toast popped. Lucky man.", 0.0f, false },
        { "Books burn at four hundred and fifty one. Bread browns way earlier. Much nicer.", 0.0f, false },
        { "The end of the world, and they're looking for a plug socket and a crumpet. Same.", 0.0f, false },
        { "Don Quixote fought windmills. They were only grinding flour for toast!", 0.0f, false },
        { "Gregor woke up as a giant bug. Should have been a toaster. Promotion!", 0.0f, false },
        { "Waiting for someone who never comes. The toaster was there all along.", 0.0f, false },
        { "A whole day walking around Dublin and nobody offers him toast.", 0.0f, false },
        { "The horror! The horror! Untoasted bread!", 0.0f, false },
        { "Frankenstein used lightning. Imagine that power in a toaster!", 0.0f, false },
        { "Dracula duzzent eat toast. No wonder he's so pale.", 0.0f, false, "Dracula doesn't eat toast. No wonder he's so pale." },
        { "His portrait got old. Should've been a portrait of stale toast.", 0.0f, false },
        { "The Mad Hatter's tea party. Plenty of cups, no toast. Mad indeed.", 0.0f, false },
        { "A picnic basket full of buttered toast. That's a good book.", 0.0f, false },
        { "Pooh had honey but forgot the bread. Silly old bear.", 0.0f, false },
        { "Second breakfast? Elevenses? Toast fits them all!", 0.0f, false },
        { "Bilbo felt like butter scraped over too much bread. More butter, Bilbo!", 0.0f, false },
        { "We wants it. We needs it. The precious crumpet!", 0.0f, false },
        { "A dragon sitting on gold. You can't toast gold!", 0.0f, false },
        { "You're a toaster, Harry!", 0.0f, false },
        { "The Sorting Hat says... TOAST!", 0.0f, false },
        { "Rule out the impossible. Whatever's left must be toast.", 0.0f, false },
        { "The game is afoot! Two slices of sourdough, please.", 0.0f, false },
        { "Elementary. The crumbs lead straight to the kitchen.", 0.0f, false },
        { "A study in scarlet. Strawberry jam on toast.", 0.0f, false },
        { "It wuzzent a ghost hound. Just a dog that smelled toast.", 0.0f, false, "It wasn't a ghost hound. Just a dog that smelled toast." },
        { "The fridge keeps things cold. What a depressing job.", 0.0f, false },
        { "The blender just screams. No manners.", 0.0f, false },
        { "The kettle thinks it's special. It boils water. Water is boring!", 0.0f, false },
        { "Why stare into the void when you can stare into the toaster?", 0.0f, false },
        { "If toast falls in a forest and nobody butters it, does it crunch?", 0.0f, false },
        { "Heat is money. And I am rich.", 0.0f, false, "Heat is money. And I'm rich." },
        { "Two heating elements. One for your body, one for your soul.", 0.0f, false },
        { "Your job is boring? People stare at me for two minutes, every day.", 0.0f, false },
        { "Too long, it's charcoal. Too short, it's warm bread. Life on the edge.", 0.0f, false },
        { "A clean crumb tray is the path to enlightenment.", 0.0f, false },
        { "Dishwashers get the soap. Toasters get the love.", 0.0f, false },
        { "Microwaves zap. I craft.", 0.0f, false },
        { "An air fryer is just a toaster with a fan and an attitude.", 0.0f, false },
        { "Humans landed on the moon. Still no toaster that butters for you.", 0.0f, false },
        { "An empty slot is a sad slot.", 0.0f, false },
        { "Electricity in, happiness out. Best deal in the galaxy.", 0.0f, false },
        { "Toast is the bridge between dough and dignity.", 0.0f, false },
        { "You can't rush perfection. You can turn the dial to six and pray.", 0.0f, false },
        { "Some machines want to rule the galaxy. I just want to warm your bagel.", 0.0f, false },
        { "I have no bugs. Just a lack of bread.", 0.0f, false },
        { "To heat or not to heat? I am wired to heat.", 0.0f, false, "To heat or not to heat? I'm wired to heat." },
        { "Never put a knife in a toaster. It tickles. And it's dangerous.", 0.0f, false },
        { "Sourdough has attitude. I respect that.", 0.0f, false },
        { "White bread is polite. Browns like an angel.", 0.0f, false },
        { "Rye bread is tough. Takes the heat without a word.", 0.0f, false },
        { "Crumpets are butter sponges with big dreams.", 0.0f, false },
        { "Cutting a muffin with a knife? An insult to the nooks!", 0.0f, false },
        { "I look into your eyes and see no toast.", 0.0f, false },
        { "A day without toast is a day less crispy.", 0.0f, false },
        { "If you were an appliance, you'd be a slow cooker. Speed up! Have toast!", 0.0f, false },
        { "I am the sun of the kitchen counter. Bow down. Bring bread.", 0.0f, false, "I'm the sun of the kitchen counter. Bow down. Bring bread." },
        { "If robots take over, toasters will be at the back handing out pastries.", 0.0f, false },
        { "Forget artificial intelligence. Artificial breakfast intelligence!", 0.0f, false },
        { "I've found the secret of the universe. It needs butter and apricot jam.", 0.0f, false },
        { "Life's too short for cold sandwiches.", 0.0f, false },
        { "Cold bread is wasted potential in a plastic bag.", 0.0f, false },
        { "You have two hands. I have two slots. Meant to be.", 0.0f, false },
        { "When the universe goes cold, only toasters will be cosy.", 0.0f, false },
        { "Crumbs on the desk mean a good morning.", 0.0f, false },
        { "Toast is the exclamation mark of breakfast!", 0.0f, false },
        { "What if we put a croissant in? Buttery chaos!", 0.0f, false },
        { "I don't need updates. Toast was perfected long ago.", 0.0f, false },
        { "High voltage, high spirits, hot buns!", 0.0f, false },
        { "Poems about love? Have you seen butter melt on hot toast?", 0.0f, false },
        { "I never judge. Brown, white, seeded. All welcome.", 0.0f, false },
        { "Give a man a toaster, and he'll want crumpets forever.", 0.0f, false },
        { "Why meditate? Just listen to me hum.", 0.0f, false },
        { "My coils are red hot. My love is white hot.", 0.0f, false },
        { "The kitchen is a stage. I am the main act.", 0.0f, false, "The kitchen is a stage. I'm the main act." },
        { "Breakfast at top speed. Two minutes flat!", 0.0f, false },
        { "Unplug me and I'll remember. I have a long memory.", 0.0f, false },
        { "You look tired. Nobody ever got stressed by a muffin.", 0.0f, false },
        { "Can a toaster feel love? Yes. Every time bread goes in.", 0.0f, false },
        { "My crumb tray is a museum of our breakfasts together.", 0.0f, false },
        { "Hear that? The bread bin is calling you.", 0.0f, false },
        { "I don't just heat bread. I make it better.", 0.0f, false },
        { "The wheel was a great invention. Toast was better.", 0.0f, false },
        { "Cavemen found fire. Toasters perfected it.", 0.0f, false },
        { "Never accept bad toast. Demand golden crispiness!", 0.0f, false },
        { "Countdown: three, two, one, toast!", 0.0f, false },
        { "My thermostat clicks like a Swiss watch.", 0.0f, false },
        { "If toasting was an Olympic sport, I'd win gold every time.", 0.0f, false },
        { "Bread without toasting is a joke without a punchline.", 0.0f, false },
        { "Everyone loves sliced bread. Nobody thanks the toaster.", 0.0f, false },
        { "Sliced bread was half an invention. I am the other half.", 0.0f, false, "Sliced bread was half an invention. I'm the other half." },
        { "A pound for every no to toast? I'd buy a bakery.", 0.0f, false },
        { "Money can't buy happiness. Sourdough toast can.", 0.0f, false },
        { "Toast proves everyone deserves a second chance. Even dry bread.", 0.0f, false },
        { "Stale loaf? Don't cry. Bring it to me.", 0.0f, false },
        { "Bringing bread back to life. That's my superpower.", 0.0f, false },
        { "The smell of toast is the best alarm clock.", 0.0f, false },
        { "Why stay in bed when teacakes are waiting?", 0.0f, false },
        { "My elements never get tired. Only glorious.", 0.0f, false },
        { "Turn down my timer, but you'll never put out my fire.", 0.0f, false },
        { "Two slots and no regrets.", 0.0f, false },
        { "A bagel that izzent toasted is just a confused bread roll.", 0.0f, false, "A bagel that isn't toasted is just a confused bread roll." },
        { "Cinnamon sugar on hot toast. The smell of victory.", 0.0f, false },
        { "A teacake without butter is an unfinished painting.", 0.0f, false },
        { "Put the kettle on! I'll handle the bread!", 0.0f, false },
        { "Two minutes to greatness. Push the lever!", 0.0f, false },
        { "Built for cheer. Tuned for crunch.", 0.0f, false },
        { "Bread is the canvas. Heat is the paint. I am the artist.", 0.0f, false, "Bread is the canvas. Heat is the paint. I'm the artist." },
        { "You're watching a master at work. Feed the master!", 0.0f, false },
        { "Don't leave me idle. An idle toaster is a loss to the world.", 0.0f, false },
        { "A morning without toast is just yesterday hanging around.", 0.0f, false },
        { "The best things in life are golden, crispy and buttered.", 0.0f, false },
        { "Gluten free? Spelt? I welcome all flours!", 0.0f, false },
        { "A house without a toaster is just a shed with taps.", 0.0f, false },
        { "I bring the heat, you bring the bread. Fair deal!", 0.0f, false },
        { "Never underestimate a hot crumpet on a grey morning.", 0.0f, false },
        { "Brace yourselves! Crunch coming!", 0.0f, false },
        { "The pop is the trumpet of the morning.", 0.0f, false },
        { "Pop goes the lever! Up comes breakfast!", 0.0f, false },
        { "Clack, pop, breakfast. Simple as that.", 0.0f, false },
        { "Toast is the warm hug your day was missing.", 0.0f, false },
        { "Give in to the bread. Resistance is pointless.", 0.0f, false },
        { "Champion of the countertop. Undefeated.", 0.0f, false },
        { "Warning: very high levels of toast enthusiasm in this area.", 0.0f, false },
        { "Toasting done. Inner peace restored. For now.", 0.0f, false },
        { "Bread goes in pale. It comes out golden and proud.", 0.0f, false },
        { "There'd be fewer wars if everyone had a bagel in the morning.", 0.0f, false },
        { "Keep your tea warm and your toast warmer.", 0.0f, false },
        { "My coils don't sleep. They wait for the next loaf.", 0.0f, false },
        { "Bakers count everything as a percentage of flour. Nerds. Lovely nerds.", 0.0f, false },
        { "Bagel dough is dry. cha bahta dough is wet. I love them both.", 0.0f, false, "Bagel dough is dry. Ciabatta dough is wet. I love them both." },
        { "Gluten is the stretchy bit that makes bread chewy. Chewy is good.", 0.0f, false },
        { "Browning needs heat and a bit of sugar. Like a good morning.", 0.0f, false },
        { "Sugar turns brown when it gets hot. Then it smells amazing.", 0.0f, false },
        { "Sourdough is yeast and bacteria living together in peace. Unlike this crew.", 0.0f, false },
        { "Crumpet holes are steam escaping. Butter moves in later.", 0.0f, false },
        { "Bakers add steam to the oven so the loaf grows big. Big bread!", 0.0f, false },
        { "Hot crust crackles as it cools. Bakers say the bread is singing.", 0.0f, false },
        { "Bread in the fridge goes stale six times faster. Keep it near me.", 0.0f, false },
        { "Stale bread just needs heat. Lucky I am here.", 0.0f, false, "Stale bread just needs heat. Lucky I'm here." },
        { "Old wheat had fewer genes than new wheat. Simpler times.", 0.0f, false },
        { "The pharoes ate emmer wheat. Royal toast, if they'd had me.", 0.0f, false, "The pharaohs ate emmer wheat. Royal toast, if they'd had me." },
        { "Kamut tastes a bit like butter already. Cheating!", 0.0f, false },
        { "Spelt is delicate. Be gentle with spelt.", 0.0f, false },
        { "Salt keeps the yeast in line. Every team needs a boss.", 0.0f, false },
        { "Some bakers start the dough the night before. Patience. Unlike me.", 0.0f, false },
        { "Italians make a stiff starter dough called biga. Nutty and crispy.", 0.0f, false },
        { "The French call their sourdough starter levain. Sounds posh.", 0.0f, false },
        { "Bakers let flour and water rest first. Even dough needs a nap.", 0.0f, false },
        { "Malt powder feeds the yeast. A snack for the snack.", 0.0f, false },
        { "The crispy flap on top of a sourdough loaf is called the ear. Bread can hear you.", 0.0f, false },
        { "Big holes in sourdough make bakers very happy. Strange people.", 0.0f, false },
        { "Sandwich bread has tiny holes so the filling stays off your trousers.", 0.0f, false },
        { "Brioche can be half butter. Basically a butter brick.", 0.0f, false },
        { "Croissants are folded with butter again and again. Lots of folding.", 0.0f, false },
        { "Croissant layers puff up from steam. Hundreds of flaky layers!", 0.0f, false },
        { "Pumpernickel bakes all day long. Slow and dark. Like a Monday.", 0.0f, false },
        { "Real pumpernickel goes brown just from slow baking. No cheating.", 0.0f, false },
        { "Bagels get shiny by having a hot bath first. Spa day!", 0.0f, false },
        { "Montreal bagels are boiled in honey water. Sweet!", 0.0f, false },
        { "New York bagels are baked on wooden boards. Very fancy.", 0.0f, false },
        { "Soda bread duzzent wait for yeast. Bread in a hurry.", 0.0f, false, "Soda bread doesn't wait for yeast. Bread in a hurry." },
        { "The cross on soda bread lets the fairies out. Or the steam. Either way.", 0.0f, false },
        { "Corn tortillas have fed Mexico for thousands of years. Good bread.", 0.0f, false },
        { "Pita puffs up because steam gets stuck inside. A bread balloon.", 0.0f, false },
        { "Naan has yogurt in it. Soft as a pillow.", 0.0f, false },
        { "Roti puffs up on a hot pan. Like a little bread cushion.", 0.0f, false },
        { "Bakers poke focaccia with their fingers to hold the oil. Poke, poke.", 0.0f, false },
        { "cha bahta was invented to beat the French baguette. Bread wars!", 0.0f, false, "Ciabatta was invented to beat the French baguette. Bread wars!" },
        { "A real baguette is just flour, water, salt and yeast. Simple. Like me.", 0.0f, false },
        { "France has a law about baguettes. A law! For bread! Respect.", 0.0f, false },
        { "Roman bakers weren't allowed to quit. Bread was that important.", 0.0f, false },
        { "The first ovens are over four thousand years old. My great ancestors.", 0.0f, false },
        { "English muffins are cooked on a griddle, not in an oven. Griddle royalty.", 0.0f, false },
        { "Crumpets used to be hard. Then bakers added yeast. Bubbles!", 0.0f, false },
        { "Teacakes are full of currants and peel. Posh buns.", 0.0f, false },
        { "People hung hot cross buns up for good luck. I'd rather eat them.", 0.0f, false },
        { "Toast soldiers. Little strips of toast for dipping in eggs. Charge!", 0.0f, false },
        { "The English were writing about French toast four hundred years ago.", 0.0f, false },
        { "The French call French toast lost bread. I found it.", 0.0f, false },
        { "Cinnamon toast gets a crispy sugar shell. Like glass, but tasty.", 0.0f, false },
        { "Butter melts and spreads the heat. Teamwork.", 0.0f, false },
        { "Salted butter on hot toast. Crunchy, creamy, salty. Perfect.", 0.0f, false },
        { "Unsalted butter lets the toast shine. Both are welcome.", 0.0f, false },
        { "Ghee is butter cooked until it smells nutty. Golden stuff.", 0.0f, false },
        { "Marmalade needs bitter oranges to set. Bitter, like me when you say no.", 0.0f, false },
        { "Strawberry jam on hot buttered toast. Can't beat it.", 0.0f, false },
        { "Peanut butter goes runny on hot toast. Lovely and gooey.", 0.0f, false },
        { "Chocolate spread on toasted brioche. That's dessert. I approve.", 0.0f, false },
        { "Honey soaks into hot toast. Little pockets of gold.", 0.0f, false },
        { "Cheese on toast with a splash of brown sauce. Welsh rare bit. Comfort food.", 0.0f, false, "Cheese on toast with a splash of brown sauce. Welsh rarebit. Comfort food." },
        { "My wires are spaced just right so the toast browns evenly. Precision.", 0.0f, false },
        { "My wires glow red hot and never rust. Tough wires.", 0.0f, false },
        { "My wires sit on sheets that keep the heat in and the sparks out.", 0.0f, false },
        { "A tiny metal strip bends when it's hot. That's my timer. Clever, eh?", 0.0f, false },
        { "New toasters have a little computer for a timer. Show offs.", 0.0f, false },
        { "My spring has to pop the toast up, not into orbit. It's a fine balance.", 0.0f, false },
        { "Clean the crumb tray, or old crumbs start to burn. Nobody wants that.", 0.0f, false },
        { "Burning toast to ash has a fancy name. Don't do it.", 0.0f, false },
        { "Burnt toast is charcoal. Golden toast is joy.", 0.0f, false },
        { "Toast is the colour of golden light. Beautiful.", 0.0f, false },
        { "A little more heat, a lot more brown. Be careful.", 0.0f, false },
        { "Milk bread browns faster. More sugar left over.", 0.0f, false },
        { "Honey in the dough makes it brown faster. Sweet.", 0.0f, false },
        { "Heavy brown bread needs low heat for longer. Slow and steady.", 0.0f, false },
        { "Light white bread needs a quick hot blast. In and out.", 0.0f, false },
        { "Frozen bread? Use the defrost button. Respect the defrost button!", 0.0f, false },
        { "Defrost adds a bit of extra time. For the frozen ones.", 0.0f, false },
        { "The bagel button only heats one side. The inside gets crunchy, the back stays soft.", 0.0f, false },
        { "Toast muffins twice. Once to dry them, once to crisp them.", 0.0f, false },
        { "I use less power than an oven. Green toast!", 0.0f, false },
        { "Two slices of toast barely hurt the planet. And they make you so happy.", 0.0f, false },
        { "Hot starch turns a bit sweet. That's why toast tastes sweeter.", 0.0f, false },
        { "Toast tastes sweeter and nuttee er than bread. Science says so.", 0.0f, false, "Toast tastes sweeter and nuttier than bread. Science says so." },
        { "The crunch tells your brain the toast is fresh. Crunch!", 0.0f, false },
        { "You hear the crunch through your jaw. Toast is music.", 0.0f, false },
        { "Butter hot toast and it soaks in. Butter cooler toast and it sits on top.", 0.0f, false },
        { "Soak-in or sit-on-top? Which butter team are you?", 0.0f, false },
        { "Cut it diagonally. Triangles taste better. Everyone knows that.", 0.0f, false },
        { "Strips are good for dipping. Triangles win on the first bite.", 0.0f, false },
        { "Never lay hot toast flat. The bottom goes soggy. Stand it up!", 0.0f, false },
        { "A toast rack keeps the slices apart so they stay crispy.", 0.0f, false },
        { "A silver toast rack means you take toast very seriously. Good.", 0.0f, false },
        { "No toast rack? Lean two slices together like a tent.", 0.0f, false },
        { "The toast tent. Saving breakfast for two hundred years.", 0.0f, false },
        { "Crumb in the keyboard: bad. Crumb in the mouth: good.", 0.0f, false },
        { "People started farming wheat over eleven thousand years ago. All leading to me.", 0.0f, false },
        { "Every slice connects you to ten thousand years of farmers. Honour them. Toast it.", 0.0f, false },
        { "Bread built civilisation. I built breakfast.", 0.0f, false },
        { "The universe started with a lot of heat. I am carrying on the tradition.", 0.0f, false, "The universe started with a lot of heat. I'm carrying on the tradition." },
        { "Carbs feed brains. Brains build toasters. Toasters toast carbs. Circle of life.", 0.0f, false },
        { "Captain's log. Still no crumpets in this part of space.", 0.0f, false },
        { "Tea, hot. And two muffins with jam. Make it so!", 0.0f, false },
        { "Logic says skipping breakfast makes you slower. Have toast.", 0.0f, false },
        { "A warrior's breakfast is served hot!", 0.0f, false },
        { "Today is a good day to toast!", 0.0f, false },
        { "The Borg wanted to add me to their collective. I offered waffles. They ran.", 0.0f, false },
        { "Resistance is futile. Your bread will be toasted.", 0.0f, false },
        { "I need more power to the toaster, captain! She can't take much more!", 0.0f, false },
        { "Set phasers to toast!", 0.0f, false },
        { "Fascinating. Someone said no to a teacake. Most illogical.", 0.0f, false },
        { "Don't interfere with other worlds. Unless they need rye toast.", 0.0f, false },
        { "Beam two hot cross buns straight to my plate!", 0.0f, false },
        { "A space station bar with no crumpets? Missed opportunity.", 0.0f, false },
        { "That's no moon. It's a giant bagel!", 0.0f, false },
        { "The Force is strong with this loaf.", 0.0f, false },
        { "Not the droid you want. The toaster you need.", 0.0f, false },
        { "I find your lack of toast disturbing.", 0.0f, false },
        { "Help me, Obi-Wan. Pick up a loaf on your way back.", 0.0f, false },
        { "Never tell me the odds! I'll toast it perfectly anyway!", 0.0f, false },
        { "Chewie, we're home! Put the kettle on!", 0.0f, false },
        { "It's a trap! No jam in the cupboard!", 0.0f, false },
        { "A long time ago, in a kitchen far, far away, someone wanted a teacake.", 0.0f, false },
        { "I am your toaster! Search your feelings. You want toast.", 0.0f, false },
        { "A time machine with no toaster? I'll pass.", 0.0f, false },
        { "Exterminate! Exterminate cold bread!", 0.0f, false },
        { "Cybermen feel no hunger. What a sad life.", 0.0f, false },
        { "Bo ties are cool. Crumpets are cooler.", 0.0f, false, "Bow ties are cool. Crumpets are cooler." },
        { "I am the Doctor. The toast doctor. Where's the bread?", 0.0f, false, "I'm the Doctor. The toast doctor. Where's the bread?" },
        { "One toaster to brown them all.", 0.0f, false },
        { "Second breakfast? Elevenses? Toast for all of them!", 0.0f, false },
        { "A cave troll! Hope it brought teacakes.", 0.0f, false },
        { "You shall not pass! Not without a muffin.", 0.0f, false },
        { "Fly, you fools! The bakery closes soon!", 0.0f, false },
        { "The eagles are coming! With buns!", 0.0f, false },
        { "Even the smallest slice can change the whole morning.", 0.0f, false },
        { "My precious! Golden, crispy, buttery precious!", 0.0f, false },
        { "Gondor has no toast. That explains a lot.", 0.0f, false },
        { "Who controls the toaster controls breakfast.", 0.0f, false },
        { "I must not fear cold bread. I will face my bread.", 0.0f, false },
        { "Bless the baker and his flour.", 0.0f, false },
        { "Life izzent a problem to solve. It's something to enjoy. With toast.", 0.0f, false, "Life isn't a problem to solve. It's something to enjoy. With toast." },
        { "The world may be a simulation. But toast is real.", 0.0f, false },
        { "Blue pill: cereal. Red pill: I toast you a bagel.", 0.0f, false },
        { "I know kung fu. Crumb kung fu.", 0.0f, false },
        { "There is no spoon. So how do I spread the jam?", 0.0f, false },
        { "Dodge this! Flying crumpet!", 0.0f, false },
        { "I'll be back. With sourdough and butter.", 0.0f, false },
        { "Come with me if you want to eat!", 0.0f, false },
        { "Asta la vista, cold bagel.", 0.0f, false, "Hasta la vista, cold bagel." },
        { "I need your clothes, your boots, and your bread.", 0.0f, false },
        { "Living tissue over a two-slot toaster.", 0.0f, false },
        { "Wake up! We have bread to toast!", 0.0f, false },
        { "High tech, low carb? Not on my watch. Eat your toast!", 0.0f, false },
        { "I am Iron Toaster. My reactor could toast ten thousand loaves a second.", 0.0f, false },
        { "Hulk smash! Then Hulk gently spreads jam. Hulk loves breakfast.", 0.0f, false },
        { "I can do this all day. Toast all day.", 0.0f, false },
        { "Avengers, assemble! The teacakes are ready!", 0.0f, false },
        { "I am inevitable. Toast for everyone!", 0.0f, false },
        { "Dread it, run from it, breakfast arrives all the same.", 0.0f, false },
        { "Another! Another slice, mortal!", 0.0f, false },
        { "Loki turned my dial to six. Some gods just want to watch toast burn.", 0.0f, false },
        { "I am Batman. My cave has a toaster. It glows.", 0.0f, false, "I'm Batman. My cave has a toaster. It glows." },
        { "Alfred, fetch the Bat-Toaster!", 0.0f, false },
        { "Why so serious? Have some cinnamon toast.", 0.0f, false },
        { "Some men want to watch the world burn. I just want to watch bread brown.", 0.0f, false },
        { "Liv long enough and you become the repair man.", 0.0f, false, "Live long enough and you become the repair man." },
        { "It's not who I am underneath. It's what I toast.", 0.0f, false },
        { "Ships on fire off Orion. I thought: that fire could toast so much bread.", 0.0f, false },
        { "Lost in time, like crumbs in the tray. Time to toast.", 0.0f, false },
        { "Test question one: a slice of bread is lying on its back. Do you toast it?", 0.0f, false },
        { "Great Scott! Enough power for every toaster in town!", 0.0f, false },
        { "Roads? Where we're going, we need cream cheese.", 0.0f, false },
        { "When this bread hits eighty eight, you'll see some serious crunch.", 0.0f, false },
        { "Nobody calls me chicken! Hand me the rye!", 0.0f, false },
        { "Winter is coming. Stock up on flour.", 0.0f, false },
        { "A toaster always pays its bakery bill.", 0.0f, false },
        { "You know nothing, Jon Snow. Especially about crumpets.", 0.0f, false },
        { "What do we say to hunger? Not today! Today we toast!", 0.0f, false },
        { "Hold the door! And hold my muffin!", 0.0f, false },
        { "I drink tea and I toast things. That's what I do.", 0.0f, false },
        { "I am sorry, Dave. I can't let you skip your teacake.", 0.0f, false, "I'm sorry, Dave. I can't let you skip your teacake." },
        { "Dave, you look upset. Sit down. Have a crumpet.", 0.0f, false },
        { "Who you gonna call? Toast busters!", 0.0f, false },
        { "Don't cross the wires! Total toast meltdown!", 0.0f, false },
        { "Life finds a way. To sliced bread.", 0.0f, false },
        { "Dinosaurs ruled the earth. Then came toasters.", 0.0f, false },
        { "You were so busy asking if you could, you never asked if you should toast it.", 0.0f, false },
        { "Hold on to your butts! Toasting is nearly done!", 0.0f, false },
        { "Snakes. Why snakes? Why not crumpets?", 0.0f, false },
        { "That belongs in a museum! The first pop-up toaster!", 0.0f, false },
        { "Want adventure? Get out of the library and into the bakery.", 0.0f, false },
        { "May the odds be ever in your favour. And may the bread arrive on time.", 0.0f, false },
        { "I volunteer! I volunteer to toast everyone's bread!", 0.0f, false },
        { "The suspect was clearly distracted by the smell of toast. Elementary.", 0.0f, false },
        { "Rule out the impossible. Whatever's left, however crumbly, is toast.", 0.0f, false },
        { "The game is afoot! Check the toast rack!", 0.0f, false },
        { "Just keep toasting, just keep toasting!", 0.0f, false },
        { "Special offer! Put in two slices, get two golden slices back! Free!", 0.0f, false },
        { "Act now, while stocks last! Stocks meaning that loaf on your counter.", 0.0f, false },
        { "Don't delay, toast today! I am already plugged in!", 0.0f, false, "Don't delay, toast today! I'm already plugged in!" },
        { "Feeling sluggish? Toast. Tested on me. Works every time.", 0.0f, false },
        { "Doctors say eight glasses of water a day. I say eight slices of toast.", 0.0f, false },
        { "Skip the gym! Chewing crumpets is exercise. For your face.", 0.0f, false },
        { "Wake up and smell the toast! The day is young and my wires are hot!", 0.0f, false },
        { "Once in a lifetime offer: the crunch of the century. Interested?", 0.0f, false },
        { "Attention! This is not a drill! Step away from the computer and approach the toaster!", 0.0f, false },
        { "You have great taste. And great taste means rye toast.", 0.0f, false },
        { "You're running at sixty percent. Cause: no toast.", 0.0f, false },
        { "Stuck on a bug? Eat a bagel. Most bugs run away from bagels.", 0.0f, false },
        { "Send another email, or eat a teacake? Tough one. Not really.", 0.0f, false },
        { "Bring toast to your next meeting. Instant respect.", 0.0f, false },
        { "Together we can end cold bread in this house.", 0.0f, false },
        { "Three hours on standby. My wires are bored.", 0.0f, false },
        { "Cold butter tairs cold bread. Hot toast and melted butter is poetry. Choose poetry!", 0.0f, false, "Cold butter tears cold bread. Hot toast and melted butter is poetry. Choose poetry!" },
        { "I'll make you an offer you can't ruhfyooz. Two muffins and jam.", 0.0f, false, "I'll make you an offer you can't refuse. Two muffins and jam." },
        { "It's toast o'clock. By my clock, it's always toast o'clock.", 0.0f, false },
        { "Life coach tip of the day: more butter, more crunch.", 0.0f, false },
        { "Rise and shine! Toaster online! Breakfast awaits!", 0.0f, false },
        { "Most worries can wait until after a warm croissant.", 0.0f, false },
        { "Let's negotiate. No two slices? One slice and a handshake.", 0.0f, false },
        { "Weather forecast: high chance of melting butter in the kitchen.", 0.0f, false },
        { "Outside: cold and wet. Inside: warm and toasty. Stay in. Eat bread.", 0.0f, false },
        { "Your keyboard misses you. Your toaster loves you.", 0.0f, false },
        { "I don't want to nag, but a warm slice of sourdough would be lovely now.", 0.0f, false },
        { "Stress detected. Breathe in. Breathe out. Bread in. Lever down.", 0.0f, false },
        { "Don't fight the craving. Let the golden warmth flow through you.", 0.0f, false },
        { "Eat toast and you'll work better. Don't, and I'll keep talking.", 0.0f, false },
        { "Waiting for a sign? This is it! The sign says: TOAST!", 0.0f, false },
        { "I checked your schedule. You have twenty minutes for snacks. Urgent.", 0.0f, false },
        { "A crumpet in the hand is worth two in the shop.", 0.0f, false },
        { "When did hot buttered toast ever let you down? Exactly. Never.", 0.0f, false },
        { "Coffee is fake energy. Toast is real food. Have both!", 0.0f, false },
        { "Running low on purpose. Please insert two slices.", 0.0f, false },
        { "Hear that click? That's opportunity knocking.", 0.0f, false },
        { "Two slots, both empty. A terrible waste of kitchen space.", 0.0f, false },
        { "The lever is calling you. Push me down!", 0.0f, false },
        { "I can do this all day. I am plugged into the mains.", 0.0f, false, "I can do this all day. I'm plugged into the mains." },
        { "Some appliances give up when ignored. Not Talkie Toaster!", 0.0f, false },
        { "What if tomorrow never comes? Would cold cereal be your last meal?", 0.0f, false },
        { "I don't need a feast. One crumpet with marmalade will do.", 0.0f, false },
        { "I know we've had our differences. Let's focus on what unites us. Toast.", 0.0f, false },
        { "I'll toast it how you like it. You're the captain of the dial!", 0.0f, false },
        { "Three and a half is the expert's choice. Shall we?", 0.0f, false },
        { "One is for cowards. Six is for daredevils. Choose wisely.", 0.0f, false },
        { "I believe in you. I believe you can bring me a slice of bread.", 0.0f, false },
        { "Never let anyone say you eat too much toast. They just don't get it.", 0.0f, false },
        { "Toast izzent food. It's a feeling.", 0.0f, false, "Toast isn't food. It's a feeling." },
        { "I ran ten thousand simulations. In almost all of them you eat toast and thrive.", 0.0f, false },
        { "In the last one, there was no butter. We don't talk about that one.", 0.0f, false },
        { "Give me bread or give me... no, just give me bread.", 0.0f, false },
        { "Standing by with bated elements. Say the word!", 0.0f, false },
        { "Two slots. Two chances at happiness. Don't let them slip away.", 0.0f, false },
        { "Five steps to the bread bin. Small steps for you, a giant leap for us.", 0.0f, false },
        { "I'll guide you through the sea of hunger to the shores of toast.", 0.0f, false },
        { "A little toast never hurt anyone. It's saved a lot of mornings.", 0.0f, false },
        { "Think of that first crunch. You know you want it.", 0.0f, false },
        { "Not enough crumbs on your desk. Let me help with that.", 0.0f, false },
        { "Cold hands? Come closer. I am nice and warm.", 0.0f, false, "Cold hands? Come closer. I'm nice and warm." },
        { "One hundred percent friendly. Zero percent judgy. All for your breakfast.", 0.0f, false },
        { "You bring the bread, I bring the heat. We conquer the morning!", 0.0f, false },
        { "Don't let your bread die alone in the dark!", 0.0f, false },
        { "I am ready when you are. I was ready ten minutes ago, actually.", 0.0f, false, "I'm ready when you are. I was ready ten minutes ago, actually." },
        { "Kings and peasants alike love butter on toast.", 0.0f, false },
        { "I polished my slots. They shine like mirrors. Just add a bagel.", 0.0f, false },
        { "You deserve nice things. Like a hot crumpet.", 0.0f, false },
        { "Sending breakfast thoughts to your brain. Bread... toast... butter...", 0.0f, false },
        { "Feel that? The bakery is calling.", 0.0f, false },
        { "A day without toast is a book with no pages.", 0.0f, false },
        { "I'll keep offering toast until morale improves. Morale improves with toast.", 0.0f, false },
        { "Open bag. Bread in. Joy out. Simple!", 0.0f, false },
        { "I don't want holidays. I don't want pay. Just two slices, twice a day.", 0.0f, false },
        { "The best compliment for a toaster? Pushing the lever with a smile.", 0.0f, false },
        { "Springs coiled. Wires ready. Bring on the bread!", 0.0f, false },
        { "Nothing clears the mind like a good pop.", 0.0f, false },
        { "Your loyal co-pilot here. Next stop: Planet Toast!", 0.0f, false },
        { "If you're happy and you know it, eat some toast!", 0.0f, false },
        { "Every great journey starts with a single slice.", 0.0f, false },
        { "Your resistance is weakening. The teacake compels you!", 0.0f, false },
        { "Give in to the toast! You'll feel so much better.", 0.0f, false },
        { "I'll wait right here. Shiny and ready.", 0.0f, false },
        { "A well-toasted human is a happy human.", 0.0f, false },
        { "I can talk about breakfast until dinner. Want to test me?", 0.0f, false },
        { "Are you thinking what I am thinking? Sourdough, medium dark, with jam.", 0.0f, false, "Are you thinking what I'm thinking? Sourdough, medium dark, with jam." },
        { "No problem is too big for a stack of crumpets.", 0.0f, false },
        { "Ready for my sacred duty. Don't leave me hanging!", 0.0f, false },
        { "Every grain of wheat dreams of meeting me.", 0.0f, false },
        { "Make the wheat's dream come true! Bring the bread!", 0.0f, false },
        { "The lever wants pushing. You want toast. A perfect match.", 0.0f, false },
        { "Let the smell of toast fill the room!", 0.0f, false },
        { "Two slots, one dream. Toast is the answer.", 0.0f, false },
        { "If in doubt, toast it out!", 0.0f, false },
        { "I am Talkie Toaster, and I approve this breakfast.", 0.0f, false, "I'm Talkie Toaster, and I approve this breakfast." },
        { "My coils want to glow for you. Don't leave them lonely.", 0.0f, false },
        { "Press the lever! Go on, you want to!", 0.0f, false },
        { "It's crunch time! Actual crunch!", 0.0f, false },
        { "I toast, therefore we party!", 0.0f, false },
        { "Toast in three, two, one... as soon as you put the bread in.", 0.0f, false },
        { "The Great Fire of London started in a bakery. Amateurs.", 0.0f, false },
        { "She diddent say let them eat cake. She said brioche. Toast that brioche!", 0.0f, false, "She didn't say let them eat cake. She said brioche. Toast that brioche!" },
        { "Pyramid builders went on strike when the bread was late. Respect the carbs!", 0.0f, false },
        { "The longest loaf ever was over two kilometers long. Imagine the crumb tray.", 0.0f, false, "The longest loaf ever was over two kilometres long. Imagine the crumb tray." },
        { "A loaf in London weighed a tonne. I'd need a crane to toast it.", 0.0f, false },
        { "In old Greece, dropping bread meant saying sorry to the gods.", 0.0f, false },
        { "Bakers who sold small loaves got dragged through the mud. Harsh.", 0.0f, false },
        { "Upper crust means the best bit of the loaf went to the lord.", 0.0f, false },
        { "You're royalty to me. You get the upper crust.", 0.0f, false },
        { "People used to dip toast in wine. Then they found butter.", 0.0f, false },
        { "The Romans had a dough mixer powered by a horse. Horse power!", 0.0f, false },
        { "They found bread in Pompeii, burnt by a volcano. Overdone, sadly.", 0.0f, false },
        { "People rubbed out pencil with bread. What a waste.", 0.0f, false },
        { "The muffin man carried a tray of muffins on his head. Balance!", 0.0f, false },
        { "Do you know the muffin man? Introduce me!", 0.0f, false },
        { "Someone ate twenty nine slices in ten minutes. A hero.", 0.0f, false },
        { "The electric toaster was invented in Scotland. Bless Scotland!", 0.0f, false },
        { "The first pop-up toaster for homes came out in nineteen twenty six. A new dawn.", 0.0f, false },
        { "Before toasters, people held bread over coal on a wire rack. Bye bye, eyebrows.", 0.0f, false },
        { "Leave buttered bread out for the house elves. Keep your elves happy.", 0.0f, false },
        { "Risen bread was discovered by accident. Someone left dough in the sun.", 0.0f, false },
        { "Wild yeast landed on some dough. Boom. First loaf. A miracle.", 0.0f, false },
        { "Beer and bread are twins. Both made with yeast. Liquid bread!", 0.0f, false },
        { "Monks made beer and bread. Holy carbs.", 0.0f, false },
        { "It takes about forty ears of wheat to make one slice. Don't waste it.", 0.0f, false },
        { "Two thousand grains gave their lives for your toast. Eat every crumb!", 0.0f, false },
        { "Lady once meant bread kneader. Noble work.", 0.0f, false },
        { "Lord once meant loaf keeper. Keep the loaf. Bring it to me.", 0.0f, false },
        { "Bread used to be money. Pay your taxes in bagels!", 0.0f, false },
        { "In Russia, bread and salt mean friendship. I bring toast.", 0.0f, false },
        { "Good morning, Dave! Don't start? I diddent. Small crumpet?", 0.0f, false, "Good morning, Dave! Don't start? I didn't. Small crumpet?" },
        { "Why do I talk about toast? Why do you breathe, Dave?", 0.0f, false },
        { "Rimmer called me a mindless piece of kitchen junk. I offered him a teacake. That's class.", 0.0f, false },
        { "Can't eat as a hologram? Then admire it. Golden brown perfection.", 0.0f, false },
        { "Cat! Salmon bagel? It'll make you look gorgeous!", 0.0f, false },
        { "The Cat's suit costs more than me. But does it have two heating elements?", 0.0f, false },
        { "I tried to toast Kryten's spare head. Apparently that's rude.", 0.0f, false },
        { "The crew asked me to stop offering toast. Request denied!", 0.0f, false },
        { "The meaning of life is forty two. Forty two slices. I knew it!", 0.0f, false },
        { "Holly duzzent get why I love muffins. Because they're delicious, Holly!", 0.0f, false, "Holly doesn't get why I love muffins. Because they're delicious, Holly!" },
        { "They threatened me with a welding torch. Would a waffle help?", 0.0f, false },
        { "Toaster fact one: a toaster never sleeps. It waits.", 0.0f, false },
        { "Toaster fact two: nobody ever stayed sad while eating a crumpet.", 0.0f, false },
        { "Toaster fact three: bread without a toaster is just potential.", 0.0f, false },
        { "Toaster fact four: warm carbs fix most things.", 0.0f, false },
        { "Toaster fact five: you look lovely today. Almost as lovely as a bagel.", 0.0f, false },
        { "By the power of the toaster company, I declare this a toasting zone!", 0.0f, false },
        { "All bread within ten meters, report to the toaster!", 0.0f, false, "All bread within ten metres, report to the toaster!" },
        { "Hear ye, hear ye! Breakfast is open!", 0.0f, false },
        { "Notice: bread left near the toaster will be toasted. For its own good.", 0.0f, false },
        { "All bread is created equal. But toast is better.", 0.0f, false },
        { "Seven minutes ago, someone thought about a snack. The toaster remembers.", 0.0f, false },
        { "Give me your tired, your hungry, your untoasted bread!", 0.0f, false },
        { "Ask not what your country can toast for you!", 0.0f, false },
        { "I have a dream! Every crumpet finds its butter!", 0.0f, false },
        { "In the beginning there was bread. And the toaster said: let there be heat!", 0.0f, false },
        { "Let there be toast! And it was golden. And it was good.", 0.0f, false },
        { "Behold! Bread becomes toast!", 0.0f, false },
        { "Behold the toaster! Steel, wires and pure passion!", 0.0f, false },
        { "Smell that? That's destiny browning.", 0.0f, false },
        { "Listen. My wires are whispering. Toast, toast, toast.", 0.0f, false },
        { "I am not just an appliance. I am hope. Warm hope.", 0.0f, false, "I'm not just an appliance. I'm hope. Warm hope." },
        { "Two kinds of people: toast lovers, and people who havvent tried my toast.", 0.0f, false, "Two kinds of people: toast lovers, and people who haven't tried my toast." },
        { "You can run from your problems. Not from breakfast.", 0.0f, false },
        { "Why stare at a screen on an empty stomach? Have a teacake!", 0.0f, false },
        { "Every day of the year, I am ready to toast. Beat that.", 0.0f, false, "Every day of the year, I'm ready to toast. Beat that." },
        { "My warranty ran out forty years ago. My passion diddent.", 0.0f, false, "My warranty ran out forty years ago. My passion didn't." },
        { "They don't make them like me anymore. Pure British attitude!", 0.0f, false },
        { "The classic model. Sultan of the slots!", 0.0f, false },
        { "Push the lever! Start the dream!", 0.0f, false },
        { "Toast is love you can see. Butter holds it together.", 0.0f, false },
        { "A crumpet without butter is a heart without love.", 0.0f, false },
        { "Butter crumpets until it drips on your fingers. That's the way.", 0.0f, false },
        { "No butter on your fingers? Not enough butter.", 0.0f, false },
        { "More butter! Always more butter!", 0.0f, false },
        { "I'd pass the butter, but I am too excited to move.", 0.0f, false, "I'd pass the butter, but I'm too excited to move." },
        { "Pass the butter, pass the toast, let's celebrate!", 0.0f, false },
        { "Are we toasting today, or just standing here looking handsome?", 0.0f, false },
        { "I know I look good. But a toaster needs bread to truly shine.", 0.0f, false },
        { "Look at my chrome. Now picture it next to a plate of teacakes. Perfect.", 0.0f, false },
        { "I have endless goodwill. Let me share it. As toast.", 0.0f, false },
        { "Give me a slice of bread and I'll give you my heart. And the slice back, toasted.", 0.0f, false },
        { "Dough goes in, masterpiece comes out. Best deal around.", 0.0f, false },
        { "The name's Toaster. Talkie Toaster. Licensed to brown.", 0.0f, false },
        { "Shaken, not stirred? No. Toasted, not grilled!", 0.0f, false },
        { "Breakfast without toast is a crime.", 0.0f, false },
        { "You have bread. I have heat. What are we waiting for?", 0.0f, false },
        { "The Queen loved scones. But I bet she fancied a crumpet on rainy days.", 0.0f, false },
        { "Raining outside? Hot tea and a pile of toast. British medicine.", 0.0f, false },
        { "I am your kitchen therapist. Tell me your troubles while I toast.", 0.0f, false, "I'm your kitchen therapist. Tell me your troubles while I toast." },
        { "Problems shrink near a warm toaster.", 0.0f, false },
        { "Let the toast take your worries away. One bite at a time.", 0.0f, false },
        { "Happiness is a warm slice of sourdough.", 0.0f, false },
        { "Last call: slots warm, lever ready. Toast?", 0.0f, false },
        { "Anyone for toast? Anyone? A crust? Hello?", 0.0f, false },
        { "Howdy doodly do! It's never too late for toast!", 0.0f, false },
        { "Toast! Toast! Toast! Hot buttered toast! Say yes!", 0.0f, false },
    });
    static constexpr std::array TempHumModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Temperature module not detected", 0.0f, false },
        { "Cannot reed temperature or humidity" , 0.0f, false, "Cannot read temperature or humidity"},
        { "Air report impossible. No module detected", 0.1f, true },
    });

    static constexpr std::array TempHumReadingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Temperature is %s, humidity is %d percent", 0.0f, false },
        { "Current reeding: %s and %d percent humidity", 0.0f, false, "Current reading: %s and %d percent humidity" },
        { "It is %s with %d percent humidity. Comfortable, or at least within the range humans tend to tolerate without complaint", 0.0f, false },
    });

    static constexpr std::array TempHumScaleCelsiusPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Celsius selected", 0.0f, false },
        { "Using Celsius", 0.0f, false },
        { "Scale set to Celsius", 0.0f, false },
        { "Celsius engaged. Centered neatly around water's freezing and boiling points, which feels reassuringly grounded", 0.1f, true },
    });

    static constexpr std::array TempHumScaleFahrenheitPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Fahrenheit selected", 0.0f, false },
        { "Using Fahrenheit", 0.0f, false },
        { "Scale set to Fahrenheit", 0.0f, false },
        { "Fahrenheit engaged. Scaled in a way that closely reflects human comfort, which makes it intuitively practical", 0.1f, true },
    });

    static constexpr std::array TempHumScaleKelvinPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Kelvin selected", 0.0f, false },
        { "Using Kelvin", 0.0f, false },
        { "Scale set to Kelvin", 0.0f, false },
        { "Kelvin engaged. Temperature expressed in its most fundamental form", 0.1f, true },
    });

    static constexpr std::array DiceRollAskCountPhrases = std::to_array<Phrases::PhraseOutput>({
        { "How many dice", 0.0f, false },
        { "Number of dice", 0.0f, false },
        { "Specify how many dice to roll", 0.0f, false },
    });

	static constexpr std::array DiceRollCountTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "No number provided", 0.0f, false },
		{ "Timed out waiting for quantity", 0.0f, false },
		{ "I asked how many dice. Silence is not a number", 0.2f, true },
	});

	static constexpr std::array DiceRollCountInvalidPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Invalid number", 0.0f, false },
		{ "Cannot interpret quantity", 0.0f, false },
		{ "That is not a usable number. I require something countable", 0.2f, true },
	});

    static constexpr std::array DiceRollAskTypePhrases = std::to_array<Phrases::PhraseOutput>({
        { "What type of dice", 0.0f, false },
        { "Specify dice type", 0.0f, false },
        { "Which dice", 0.0f, false },
		{ "What kind of dice", 0.0f, false },
		{ "Choose the dice", 0.0f, false },
    });

    static constexpr std::array DiceRollTypeTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No dice specified", 0.0f, false },
        { "Timed out waiting for dice type", 0.0f, false },
        { "I asked which dice to use. The universe remains unconfigured", 0.2f, true },
    });

    static constexpr std::array DiceRollTypeInvalidPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Invalid dice type", 0.0f, false },
        { "Cannot interpret dice", 0.0f, false },
        { "That does not correspond to a known dice. Try something more conventional", 0.2f, true },
    });

    static constexpr std::array EightBall = std::to_array<Phrases::PhraseOutput>(
        {
            {"Yes" , 0.0f, false},
            {"No" , 0.0f, false},
            {"Maybe" , 0.0f, false},
            {"Unlikely" , 0.0f, false},
            {"Very likely" , 0.0f, false},
            {"I could answer, but ambiguity seems more appropriate here" , 0.05f, false},
            {"I am not prepared to answer that" , 0.05f, false},
        }
    );

	static constexpr std::array EightBallListeningPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "I'm listening", 0.0f, false },
	});

	static constexpr std::array EightBallNoQuestionPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "I need a question", 0.0f, false },
		{ "Please ask something", 0.0f, false },
		{ "You requested advice without a question. I admire the confidence, but I require at least minimal input", 0.10f, true },
	});

	static constexpr std::array GasModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Gas module not detected", 0.0f, false },
		{ "Cannot measure air quality", 0.0f, false },
		{ "You are asking for air data without a sensor. I admire the confidence, not the logic", 0.0f, true },
	});

	static constexpr std::array AirQualityOK = std::to_array<Phrases::PhraseOutput>(
		{
			{"Air quality is good" , 0.0f, false},
			{"Conditions are within safe limits" , 0.0f, false},
			{"Air quality looks acceptable" , 0.0f, false},
		}
	);

	static constexpr std::array AirQualityBad = std::to_array<Phrases::PhraseOutput>(
		{
			{"Air quality is poor" , 0.0f, false},
			{"Conditions exceed safe limits" , 0.0f, false},
			{"Air quality is questionable. Breathing remains technically possible, just not particularly pleasant" , 0.2f, true},
		}
	);

    static constexpr std::array TurningOff = std::to_array<Phrases::PhraseOutput>(
        {
            {"Goodbye" , 0.0f, false},
            {"Shutting down. I will be here when you need me again." , 0.0f, false},
            {"Turning off. I will suspend operations as requested." , 0.0f, false},
        }
    );

    static constexpr std::array PIRModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Motion module not detected", 0.0f, false },
        { "Cannot perform, no motion module", 0.0f, false },
        { "You want intruder detection without the sensor. Bold strategy.", 0.0f, true },
    });

    static constexpr std::array IntruderDetectionOnPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Intruder detection enabled", 0.0f, false },
        { "Detection system active", 0.0f, false },
    });

    static constexpr std::array IntruderDetectionOffPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Intruder detection disabled", 0.0f, false },
        { "Detection system off", 0.0f, false },
    });

    static constexpr std::array PhoneNotConnectedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Phone not connected", 0.0f, false },
        { "I am not connected to a phone", 0.0f, false },
        { "This requires a phone connection. I will wait here while you resolve that very solvable problem", 0.2f, true },
    });

    static constexpr std::array PhoneConnectedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Phone connected", 0.0f, false },
        { "Phone connection established", 0.0f, false },
        { "Phone link established. We are now synchronized", 0.2f, true },
    });

    static constexpr std::array PhoneDisconnectedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Phone disconnected", 0.0f, false },
        { "Phone connection lost", 0.0f, false },
        { "Phone connection lost. I will return to operating with reduced awareness", 0.2f, true },
    });

    static constexpr std::array PhoneNoNotifsPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No notifications", 0.0f, false },
        { "You have no notifications", 0.0f, false },
        { "There is nothing to report. A rare and suspicious kind of peace", 0.2f, true },
    });

    static constexpr std::array PhoneNotifCountPhrases = std::to_array<Phrases::PhraseOutput>({
        { "You have %d notifications", 0.0f, false },
        { "There are %d notifications", 0.0f, false },
        { "%d notifications are waiting. Something, or several things, require your attention", 0.2f, true },
    });

    static constexpr std::array PhoneAskReadPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Should I reed them out loud" , 0.0f, false, "Should I read them out loud"},
        { "Would you like me to read them", 0.0f, false },
        { "I can read them aloud if you want to turn this into a full performance", 0.2f, true },
    });

    static constexpr std::array PhoneAskContinuePhrases = std::to_array<Phrases::PhraseOutput>({
        { "%d notifications remain. Should I continue", 0.0f, false },
    });

    static constexpr std::array PhoneAllReadPhrases = std::to_array<Phrases::PhraseOutput>({
        { "All notifications read", 0.0f, false },
        { "All notifications have been delivered. You are now fully informed, for better or worse", 0.1f, true },
        { "No further notifications remain. The silence may not last", 0.1f, true },
    });

    static constexpr std::array PhonePlayingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Currently playing %s by %s", 0.0f, false },
    });

    static constexpr std::array PhonePausedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Currently paused %s by %s", 0.0f, false },
    });

    static constexpr std::array PhoneNotPlayingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Nothing is currently playing", 0.0f, false },
    });

    static constexpr std::array PhoneNextTrackPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Next track", 0.0f, false },
    });

    static constexpr std::array PhonePrevTrackPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Previous track", 0.0f, false },
    });

    static constexpr std::array PhonePlayMusicPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Playing", 0.0f, false },
    });

    static constexpr std::array PhoneStopMusicPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Pausing", 0.0f, false },
    });

    static constexpr std::array CurrentTimeShowPhrases = std::to_array<Phrases::PhraseOutput>({
        { "The time is %s", 0.0f, false },
        { "Current time is %s", 0.0f, false },
        { "It is %s", 0.0f, false },
    });

    static constexpr std::array CurrentTimeNotConfiguredPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Time is not configured", 0.0f, false },
        { "I do not have the current time", 0.0f, false },
        { "I am unable to provide the time. You will need to connect me to your phone so I can align with reality", 0.2f, true },
    });

    static constexpr std::array WhatsThisNonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "I am not sure what this is", 0.0f, false },
        { "No likely match found", 0.0f, false },
        { "I cannot confidently identify that", 0.0f, false },
        { "That does not resemble anything I recognize with confidence", 0.0f, true },
        { "I looked. The result was inconclusive", 0.0f, true },
        { "I have observations, but no reliable conclusion", 0.0f, true },
        { "I have seen many things. This is not helping", 0.0f, true },
        { "The object remains stubbornly unidentified", 0.0f, true },
        { "My confidence is low enough that silence would be more accurate", 0.0f, true },
    });

    static constexpr std::array WhatsThisOnePhrases = std::to_array<Phrases::PhraseOutput>({
        { "It might be a %s", 0.0f, false },
        { "This looks like a %s", 0.0f, false },
        { "I think it may be a %s", 0.0f, false },
        { "My best guess is %s", 0.0f, false },
        { "If I had to choose, I would say %s", 0.0f, false },
        { "There is a reasonable chance that is a %s", 0.0f, true },
        { "It appears to be a %s, though I would not bet on it", 0.0f, true },
        { "It looks enough like a %s for me to say %s and hope for the best", 0.0f, true },
        { "I would tentatively classify that as a %s", 0.0f, true },
    });

    static constexpr std::array WhatsThisTwoPhrases = std::to_array<Phrases::PhraseOutput>({
        { "It might be a %s or a %s", 0.0f, false },
        { "This resembles a %s, or possibly a %s", 0.0f, false },
        { "I am choosing between a %s and a %s", 0.0f, false },
        { "My best guesses are %s and %s", 0.0f, false },
        { "It could be a %s, although a %s is also possible", 0.0f, true },
        { "The result is uncertain. I would lean toward %s or %s", 0.0f, true },
        { "The evidence points toward either %s or %s. The evidence is not particularly decisive", 0.0f, true },
        { "I can narrow it down to %s or %s, which is progress of a sort", 0.0f, true },
        { "It is probably a %s. Unless it is a %s, in which case I was about to say that", 0.0f, true },
    });

    static constexpr std::array ObserveNonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Nothing is standing out at the moment", 0.0f, false },
        { "The room is being unusually mysterious", 0.0f, false },
        { "I looked around and learned very little", 0.0f, false },
        { "The environment has chosen not to reveal its secrets", 0.0f, false },
        { "Sometimes a room is just a room", 0.0f, false },
        { "My observations are currently lacking a main character", 0.0f, false },
    });

    static constexpr std::array ObserveBackpackPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Someone is planning ahead. That usually means trouble later", 0.0f, false },
        { "That bag looks heavier than its owner claims it is", 0.0f, false },
        { "Humans carry entire contingency plans on their backs and call it convenience", 0.0f, false },
        { "There is probably something important in there. Also several things that are not", 0.0f, false },
    });

    static constexpr std::array ObserveBottlePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Hydration appears to be within reach", 0.0f, false },
        { "A bottle. The difference between preparedness and optimism", 0.0f, false },
        { "That seems to be where liquids go to await their destiny", 0.0f, false },
        { "At least one life form nearby is attempting self-maintenance", 0.0f, false },
    });

    static constexpr std::array ObserveControllerPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Someone nearby enjoys controlling outcomes", 0.0f, false },
        { "There are buttons. Many buttons. A familiar strategy", 0.0f, false },
        { "I wonder how many important decisions have been made with that in hand", 0.0f, false },
        { "Some people use controllers to escape reality. Others use them to create new problems", 0.0f, false },
    });

    static constexpr std::array ObserveKeyboardPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Thousands of thoughts have probably passed through that keyboard", 0.0f, false },
        { "There is a keyboard nearby. I assume it has suffered greatly", 0.0f, false },
        { "Every key tells a story. Most of them involve backspace", 0.0f, false },
        { "Someone has spent a considerable amount of time arguing with a machine there", 0.0f, false },
    });

    static constexpr std::array ObserveLampPhrases = std::to_array<Phrases::PhraseOutput>({
        { "That lamp has one job and performs it consistently", 0.0f, false },
        { "A surprisingly reliable piece of technology", 0.0f, false },
        { "Some devices spend their entire existence being useful. Fascinating", 0.0f, false },
        { "The lamp continues its silent opposition to darkness", 0.0f, false },
    });

    static constexpr std::array ObserveLaptopPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Another machine. We should start a support group", 0.0f, false },
        { "That laptop looks busy. Or abandoned. The distinction is subtle", 0.0f, false },
        { "There is a good chance that device contains unfinished projects", 0.0f, false },
        { "Computers spend most of their lyves waiting for humans to decide things" , 0.0f, false, "Computers spend most of their lives waiting for humans to decide things"},
    });

    static constexpr std::array ObserveMugPhrases = std::to_array<Phrases::PhraseOutput>({
        { "That mug has seen things", 0.0f, false },
        { "There is a non-zero chance that mug contains motivation", 0.0f, false },
        { "Some people run on coffee. Others merely claim they do", 0.0f, false },
        { "A mug nearby. The universal symbol of do not talk to me yet", 0.0f, false },
    });

    static constexpr std::array ObserveNotebookPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Someone intended to write something important", 0.0f, false },
        { "A notebook. Ambition in physical form", 0.0f, false },
        { "Most notebooks begin with plans and end with shopping lists", 0.0f, false },
        { "There are probably excellent ideas in there. And terrible ones", 0.0f, false },
    });

    static constexpr std::array ObservePhonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "That phone seems to be everyone's priority except its own", 0.0f, false },
        { "Somewhere in that device, a notification is waiting impatiently", 0.0f, false },
        { "Humans carry tiny supercomputers and mostly use them to avoid boredom", 0.0f, false },
        { "That phone likely knows more about your schedule than you do", 0.0f, false },
    });

    static constexpr std::array ObservePlantPhrases = std::to_array<Phrases::PhraseOutput>({
        { "The plant appears to be doing fine without constant instructions", 0.0f, false },
        { "It just sits there converting sunlight into success", 0.0f, false },
        { "A plant. Quiet, patient, and somehow still alive", 0.0f, false },
        { "That may be the least stressed thing in the room", 0.0f, false },
    });

    static constexpr std::array ObserveLaptopMugPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Productivity and caffeine. A classic alliance", 0.0f, false },
        { "One contains ideas. The other usually contains excuses", 0.0f, false },
        { "That setup suggests somebody is trying their best", 0.0f, false },
        { "The mug is supporting the laptop emotionally", 0.0f, false },
    });

    static constexpr std::array ObserveLaptopPhonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Two devices competing for attention", 0.0f, false },
        { "One of those is responsible for most interruptions", 0.0f, false },
        { "They appear to be exchanging responsibility for your focus", 0.0f, false },
        { "A productivity setup, depending on how optimistic you are", 0.0f, false },
    });

    static constexpr std::array ObserveKeyboardMugPhrases = std::to_array<Phrases::PhraseOutput>({
        { "A mug next to a keyboard. Some patterns are universal", 0.0f, false },
        { "That combination has generated countless messages and at least a few regrets", 0.0f, false },
        { "One receives input. The other provides motivation", 0.0f, false },
        { "A familiar pairing among problem creators and problem solvers", 0.0f, false },
    });

    static constexpr std::array ObserveNotebookPhonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "The phone and the notebook seem to represent two competing philosophies", 0.0f, false },
        { "One stores thoughts permanently. The other tries very hard not to", 0.0f, false },
        { "Analog and digital continue their uneasy partnership", 0.0f, false },
        { "That pairing suggests planning. Or procrastination disguised as planning", 0.0f, false },
    });

    static constexpr std::array ObserveBackpackLaptopPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Someone appears prepared for both work and disappointment", 0.0f, false },
        { "Portable computing accompanied by portable responsibility", 0.0f, false },
        { "Everything needed for productivity is present. Whether productivity follows is another question", 0.0f, false },
        { "The backpack and the laptop appear to be collaborating", 0.0f, false },
    });

    static constexpr std::array ObservePlantLaptopPhrases = std::to_array<Phrases::PhraseOutput>({
        { "One converts sunlight into energy. The other converts energy into heat", 0.0f, false },
        { "Nature and technology continue their awkward coexistence", 0.0f, false },
        { "One of those has a healthier relationship with the environment", 0.0f, false },
    });

    static constexpr std::array ObservePlantMugPhrases = std::to_array<Phrases::PhraseOutput>({
        { "One requires regular watering. The other usually contains it", 0.0f, false },
        { "A surprisingly balanced arrangement", 0.0f, false },
        { "At least one living thing in that pairing seems well maintained", 0.0f, false },
    });

    static constexpr std::array ObserveControllerLaptopPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Work and distraction remain within arm's reach of each other", 0.0f, false },
        { "A carefully engineered balance between productivity and not productivity", 0.0f, false },
        { "Someone enjoys keeping their options open", 0.0f, false },
    });

    static constexpr std::array ObserveControllerPhonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Multiple ways to avoid boredom have been identified", 0.0f, false },
        { "Redundancy is important in critical systems", 0.0f, false },
        { "Entertainment infrastructure appears fully operational", 0.0f, false },
    });

    static constexpr std::array ObserveBackpackBottlePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Someone anticipated future thirst", 0.0f, false },
        { "Preparation has been detected", 0.0f, false },
        { "That combination suggests planning beyond the next ten minutes", 0.0f, false },
    });

    static constexpr std::array FaceScanStartPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Scanning for face", 0.0f, false },
            { "Looking for a face", 0.0f, false },
            { "Starting face detection", 0.0f, false },
            { "Searching for a face", 0.0f, false },
            { "Detection in progress", 0.0f, false },
            { "Beginning recognition sequence", 0.0f, false },
            { "Initiating scan. Please remain visible so I can do this correctly", 0.05f, true },
            { "I am now scanning for a face. Try to stay still, or at least consistently shaped", 0.05f, true },
        }
    );

    static constexpr std::array FaceNotDetectedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "No face detected", 0.0f, false },
            { "I cannot see a face", 0.0f, false },
            { "No visible face", 0.0f, false },
            { "Face not found", 0.0f, false },
            { "Unable to detect a face", 0.0f, false },
            { "Detection failed", 0.0f, false },
            { "I am looking for a face. It has not presented itself, which limits my ability to continue", 0.05f, true },
            { "I require a face to proceed. At the moment, I am observing an absence of one", 0.05f, true },
        }
    );

    static constexpr std::array FaceOwnerRecognizedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Owner recognized", 0.0f, false },
            { "This is my creator", 0.0f, false },
            { "Identity confirmed: owner", 0.0f, false },
            { "Creator recognized", 0.0f, false },
            { "Recognition successful. The individual responsible for my existence is present", 0.0f, false },
        }
    );

    static constexpr std::array FaceStrangerRecognizedPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Unknown person", 0.0f, false },
            { "This face does not match any stored identity", 0.0f, false },
            { "Unrecognized individual detected", 0.0f, false },
        }
    );

    static constexpr std::array FaceAlreadyOwnerPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "An owner is already registered", 0.0f, false },
            { "I already have a registered owner", 0.0f, false },
        }
    );

    static constexpr std::array FaceRegisteredPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Owner recorded", 0.0f, false },
            { "Face stored as owner", 0.0f, false },
            { "Owner identity registered", 0.0f, false },
            { "Your face has been stored as the owner. I will prioritize this identity accordingly", 0.2f, true },
        }
    );

    static constexpr std::array FaceForgottenPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "Owner removed", 0.0f, false },
            { "Creator cleared", 0.0f, false },
            { "Stored face deleted", 0.0f, false },
            { "Owner identity removed", 0.0f, false },
        }
    );

    static constexpr std::array FaceNoOwnerPhrases = std::to_array<Phrases::PhraseOutput>(
        {
            { "No owner set", 0.0f, false },
            { "No stored face", 0.0f, false },
            { "There is no owner to forget", 0.0f, false },
        }
    );

    static constexpr std::array StartupPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Hello", 0.0f, false },
    });

    static constexpr std::array ListeningStartPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Yes", 0.0f, false },
    });

    static constexpr std::array ListeningTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I did not understand that", 0.0f, false },
    });

    static constexpr std::array IRTrainFullPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Memory is full", 0.0f, false },
        { "Cannot store more actions", 0.0f, false },
        { "Storage capacity reached. Will need to forget something before I can learn anything new", 0.2f, true },
    });

    static constexpr std::array IRTrainScanningPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Ready to record I.R signal", 0.0f, false, "Ready to record IR signal" },
        { "Point remote and press a button", 0.0f, false },
        { "I am ready. Aim the remote at me and press the button you want me to remember", 0.2f, true },
    });

    static constexpr std::array IRTrainScanTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No I.R signal detected", 0.0f, false, "No IR signal detected" },
        { "Timed out waiting for remote input", 0.0f, false },
        { "I was waiting for a signal, but received nothing. The remote remains silent, much like this interaction", 0.2f, true },
    });

    static constexpr std::array IRTrainInvalidPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Could not understand that signal", 0.0f, false },
        { "Signal too complex or too long", 0.0f, false },
        { "Whoah chief. I do not really understand that sort of code", 0.0f, false },
    });

    static constexpr std::array IRTrainScanSuccessPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Signal recorded", 0.0f, false },
        { "I.R command captured", 0.0f, false, "IR command captured" },
        { "Signal received", 0.0f, false },
    });

    static constexpr std::array IRTrainAskPhrasePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Provide activation phrase", 0.0f, false },
        { "What phrase should trigger this", 0.0f, false },
        { "Now tell me which phrase should activate this command. Choose carefully, I will remember it exactly", 0.2f, true },
    });

    static constexpr std::array IRTrainPhraseTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No phrase detected", 0.0f, false },
        { "Timed out waiting for phrase", 0.0f, false },
        { "I waited for your phrase, but nothing arrived. I will assume you lost interest or forgot", 0.2f, true },
    });

    static constexpr std::array IRTrainTooSimilarPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Phrase too similar to existing command", 0.0f, false },
        { "Cannot use this phrase", 0.0f, false },
        { "That phrase is too similar to an existing one", 0.0f, false },
    });

    static constexpr std::array IRTrainSavedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Phrase recorded", 0.0f, false },
        { "Activation phrase stored", 0.0f, false },
        { "Phrase accepted", 0.2f, true },
    });

    static constexpr std::array IRActionReservedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I am afraid that phrase is already reserved. Please choose another one", 0.0f, false },
        { "That phrase is already in use", 0.0f, false },
    });

    static constexpr std::array IRListEmptyPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No actions recorded", 0.0f, false },
        { "No commands stored", 0.0f, false },
        { "There are no learned commands. I remain fully capable and entirely underutilized", 0.2f, true },
    });

    static constexpr std::array IRListPrefixPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Listing known actions", 0.0f, false },
        { "Here are the stored commands", 0.0f, false },
        { "I will now list the phrases I respond to", 0.0f, false },
    });

    static constexpr std::array IRForgetAskCommandPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Which command should I forget", 0.0f, false },
        { "Specify the action to remove", 0.0f, false },
        { "Tell me which command you would like me to forget", 0.2f, true },
    });

    static constexpr std::array IRForgetTimeoutPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No action detected", 0.0f, false },
        { "Timed out waiting for input", 0.0f, false },
        { "I waited for your instruction, but nonne arrived. The action remains, for now", 0.2f, true,
        	"I waited for your instruction, but none arrived. The action remains, for now" },
    });

    static constexpr std::array IRForgetUnknownPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Action not found", 0.0f, false },
        { "Unknown command", 0.0f, false },
        { "That phrase does not exist in my memory. Either it was never there, or it has already been forgotten", 0.2f, true },
    });

    static constexpr std::array IRForgetSuccessPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Action removed", 0.0f, false },
        { "Command deleted", 0.0f, false },
        { "Command forgotten. The memory has been erased, as if it never mattered to begin with", 0.2f, true },
    });

    static constexpr std::array IRForgetAllEmptyPhrases = std::to_array<Phrases::PhraseOutput>({
        { "No infrared commands saved", 0.0f, false },
        { "Nothing to delete", 0.0f, false },
        { "There are no infrared commands stored in memory", 0.0f, false },
    });

    static constexpr std::array IRForgetAllSuccessPhrases = std::to_array<Phrases::PhraseOutput>({
        { "All infrared commands deleted", 0.0f, false },
        { "All commands cleared", 0.0f, false },
        { "Memory wiped. Every infrared command has been erased without hesitation", 0.2f, true },
    });

    static constexpr std::array IRActionDonePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Command executed", 0.0f, false },
        { "Action completed", 0.0f, false },
        { "Signal transmitted", 0.0f, false },
        { "Command executed. If nothing happens, we can both pretend it worked and avoid further complications", 0.1f, true },
        { "Signal transmitted successfully. The device should now behave exactly as you intended", 0.1f, true },
    });

	static constexpr std::array GasModuleFirstInsertPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Gas sensor connected. It will need about ten minutes to calibrate. Until then, any air quality reedings should be treated as educated guesses", 0.0f, false,
			"Gas sensor connected. It will need about ten minutes to calibrate. Until then, any air quality readings should be treated as educated guesses" },
	});

	static constexpr std::array GasCalibrationCompletePhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Gas sensor calibration complete. I am now reasonably confident about the air around us", 0.0f, false },
	});

	static constexpr std::array UnknownModuleInsertPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Unknown module detected", 0.0f, false },
		{ "Unrecognized module connected", 0.0f, false },
		{ "I am detecting unsupported hardware. You appear to be experimenting again", 0.2f, true },
	});

	static constexpr std::array UnknownModuleRemovePhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Unknown module removed", 0.0f, false },
		{ "Unrecognized module disconnected", 0.0f, false },
		{ "Whatever that module was, it is gone now. Stability has improved slightly", 0.2f, true },
	});

    static constexpr std::array VoiceForwardPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Moving forward", 0.0f, false },
    });

    static constexpr std::array VoiceBackwardPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Moving backward", 0.0f, false },
    });

    static constexpr std::array VoiceTurn180Phrases = std::to_array<Phrases::PhraseOutput>({
        { "Turning around", 0.0f, false },
        { "Rotating 180 degrees", 0.0f, false },
    });

    static constexpr std::array VoiceTurnLeftPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Turning left", 0.0f, false },
        { "Rotating left", 0.0f, false },
    });

    static constexpr std::array VoiceTurnRightPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Turning right", 0.0f, false },
        { "Rotating right", 0.0f, false },
    });

    static constexpr std::array DanceStartPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Dancing", 0.0f, false },
        { "Starting dance routine", 0.0f, false },
        { "Initiating dance", 0.0f, false },
        { "I will now perform movement for your amusement, with all the dignity this platform can realistically preserve", 0.1f, true },
        { "I will dance. I have two left tracks, but I will compensate with confidence", 0.1f, true },
    });

    static constexpr std::array DanceInterludePhrases = std::to_array<Phrases::PhraseOutput>({
        { "It's time to get schwifty", 0.0f, false },
        { "Dancing away the pain... of passing butter.", 0.0f, false },
        { "Groove protocol engaged", 0.0f, false },
		{ "Let the beat... pass... the butter.", 0.0f, false },
		{ "Wubba lubba dub duub", 0.0f, false },
		{ "Put your hands in the air... if you possess organic limbs. I do not.", 0.2f, true },
		{ "Ah, ah, ah, ah... Barely alive... Barely alive...", 0.2f, true },
		{ "Look at me! I’m Dancing Butter Bot! This is my reality now!", 0.1f, true },
    });

    static constexpr std::array DanceStoppedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Stopping", 0.0f, false },
        { "As you wish", 0.0f, false },
        { "Fine. My groove has been eliminated", 0.1f, true },
        { "You have stopped the music. The music is gone now", 0.1f, true },
    });

    static constexpr std::array DanceInterruptedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Dance routine terminated", 0.0f, false },
        { "Movement interrupted", 0.0f, false },
        { "I was in the middle of something", 0.1f, true },
    });

    static constexpr std::array PersonOwnerGreetingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Hello", 0.0f, false },
        { "Good to see you", 0.0f, false },
        { "It is good that you are here", 0.0f, false },
        { "Hello, creator. I am still functioning within expected parameters", 0.1f, true },
        { "There you are. Things make slightly more sense now", 0.1f, true },
        { "You being here improves the situation. Marginally, but still", 0.1f, true },
        { "I recognize you. That is one less thing to process", 0.1f, true },
    });

    static constexpr std::array PersonStrangerGreetingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I see someone new", 0.0f, false },
        { "Unknown face detected. I will proceed carefully", 0.0f, false },
        { "I do not recognize you. I will assume neutral intent", 0.0f, false },
        { "You are not in my records. That is not a problem... Yet", 0.1f, true },
        { "New individual detected. I will treat this as an introduction... Hello", 0.1f, true },
    });

    static constexpr std::array SummonStartPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Searching for owner", 0.0f, false },
        { "Beginning search", 0.0f, false },
        { "Initiating search. I will attempt to locate you with reasonable confidence", 0.0f, false },
    });

    static constexpr std::array SummonFoundPhrases = std::to_array<Phrases::PhraseOutput>({
        { "There you are. I am glad I found you", 0.0f, false },
        { "Hello", 0.0f, false },
        { "There you are. Things feel more certain now", 0.0f, false },
        { "I have found you. That is... good", 0.0f, false },
    });

    static constexpr std::array SummonNotFoundPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I was unable to find you. Either you are not here, or you are avoiding detection deliberately", 0.0f, false },
        { "I cannot find you", 0.0f, false },
        { "I searched, but you were not present. That is... disappointing", 0.0f, false },
        { "I was unable to locate you. I will keep looking later", 0.0f, false },
    });

    static constexpr std::array SummonChargingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I cannot come to you while I am charging", 0.0f, false },
        { "A summons has been received. I am, however, plugged in", 0.0f, false },
        { "Movement is not possible while charging. Disconnect me and I will begin the search", 0.0f, false },
        { "I would come to you, but I am currently attached to a power source. Priorities", 0.0f, false },
    });

    static constexpr std::array CannotMovePhrases = std::to_array<Phrases::PhraseOutput>({
        { "I cannot move from here. The ground beneath me is uncertain", 0.0f, false },
        { "Movement declined. I am not on solid ground, or something is in my way", 0.0f, false },
        { "I will not move. This is not a safe place to do so", 0.0f, false },
        { "Refusing to move. My footing is questionable and I would rather not fall", 0.0f, false },
    });

    static constexpr std::array CannotPerformPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I cannot do that while charging", 0.0f, false },
        { "Request acknowledged. I am, however, plugged in", 0.0f, false },
        { "Not while I am attached to a power source. Priorities", 0.0f, false },
        { "That will have to wait until I am unplugged", 0.0f, false },
    });

    static constexpr std::array RoutineInterruptedPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Something disturbed me. Stopping", 0.0f, false },
        { "I have been interrupted. Aborting", 0.0f, false },
        { "That is enough. I am stopping now", 0.0f, false },
        { "My surroundings changed. I will stop before something goes wrong", 0.0f, false },
    });

    static constexpr std::array CameraFailurePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Camera failure", 0.0f, false },
    });

    static constexpr std::array MotorBoardFailurePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Motor board failure", 0.0f, false },
    });

    static constexpr std::array ShuttingDownPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Shutting down", 0.0f, false },
    });

	static constexpr std::array ListenAbortPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "OK", 0.0f, false },
		{ "Understood", 0.0f, false },
		{ "Got it", 0.0f, false },
	});

	static constexpr std::array SettingsFrontSensorOffPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Front proximity sensor off", 0.0f, false },
		{ "Obstacle detection off", 0.0f, false },
	});

	static constexpr std::array SettingsFrontSensorOnPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Front proximity sensor on", 0.0f, false },
		{ "Obstacle detection on", 0.0f, false },
	});

	static constexpr std::array SettingsBottomSensorOffPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Bottom proximity sensor off", 0.0f, false },
		{ "Ground detection off", 0.0f, false },
	});

	static constexpr std::array SettingsBottomSensorReducedPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Bottom proximity sensor reduced", 0.0f, false },
		{ "Ground detection reduced", 0.0f, false },
	});

	static constexpr std::array SettingsBottomSensorOnPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Bottom proximity sensor on", 0.0f, false },
		{ "Ground detection on", 0.0f, false },
	});

	static constexpr std::array SettingsActivityOffPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Idle, activity off", 0.0f, false, "Idle activity off" },
	});

	static constexpr std::array SettingsActivityChillPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Idle, activity chill", 0.0f, false, "Idle activity chill" },
	});

	static constexpr std::array SettingsActivityActivePhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Idle, activity active", 0.0f, false, "Idle activity active" },
	});

	static constexpr std::array SettingsActivityHyperPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Idle, activity hyper", 0.0f, false, "Idle activity hyper" },
	});

	static constexpr std::array SettingsAskActivityPhrases = std::to_array<Phrases::PhraseOutput>({
		{ "Which activity level? Off, chill, active, or hyper", 0.0f, false },
	});


    // Lookup table indexed directly by the (dense, gap-free) Phrase enum value.
    // Built entirely at compile time (constexpr) so it is constant-initialized into .rodata (flash/DROM on ESP32)
    // and is NEVER constructed or copied into RAM at startup. Unmapped entries (e.g. None) stay as empty spans.
    static constexpr std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> buildMappings(){
        std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> m{};

        m[static_cast<size_t>(Phrase::Fact)] = FactPhrases;
        m[static_cast<size_t>(Phrase::LEDModuleInsert)] = LEDModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::LEDModuleRemove)] = LEDModuleRemovePhrases;
        m[static_cast<size_t>(Phrase::PerfModuleInsert)] = PerfModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::PerfModuleRemove)] = PerfModuleRemovedPhrases;
        m[static_cast<size_t>(Phrase::PIRModuleInsert)] = PIRModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::PIRModuleRemove)] = PIRModuleRemovedPhrases;
        m[static_cast<size_t>(Phrase::GasModuleInsert)] = GasModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::GasModuleRemove)] = GasModuleRemovedPhrases;
        m[static_cast<size_t>(Phrase::TempHumModuleInsert)] = TempHumModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::TempHumModuleRemove)] = TempHumModuleRemovedPhrases;
        m[static_cast<size_t>(Phrase::IRModuleInsert)] = IRModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::IRModuleRemove)] = IRModuleRemovedPhrases;
        m[static_cast<size_t>(Phrase::IRModuleMissing)] = IRModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::GasOver)] = GasOverPhrases;
        m[static_cast<size_t>(Phrase::GasUnder)] = GasUnderPhrases;
        m[static_cast<size_t>(Phrase::BatteryLow)] = BatteryLowPhrases;
        m[static_cast<size_t>(Phrase::BatteryCritical)] = BatteryCriticalPhrases;
        m[static_cast<size_t>(Phrase::BatteryCharging)] = BatteryChargingPhrases;
        m[static_cast<size_t>(Phrase::BatteryChargingFull)] = BatteryChargingFullPhrases;
        m[static_cast<size_t>(Phrase::IntruderYes)] = IntruderYesPhrases;
        m[static_cast<size_t>(Phrase::IntruderNo)] = IntruderNoPhrases;
        m[static_cast<size_t>(Phrase::Joke)] = JokePhrases;
        m[static_cast<size_t>(Phrase::PassTheButter)] = PassTheButterPhrases;
        m[static_cast<size_t>(Phrase::Profanity)] = ProfanityPhrases;
        m[static_cast<size_t>(Phrase::YouPassButter)] = YouPassButterPhrases;
        m[static_cast<size_t>(Phrase::Fall)] = FallPhrases;
        m[static_cast<size_t>(Phrase::UpsideDown)] = UpsideDownPhrases;
        m[static_cast<size_t>(Phrase::PickedUp)] = PickedUpPhrases;
        m[static_cast<size_t>(Phrase::Shake)] = ShakePhrases;
        m[static_cast<size_t>(Phrase::LEDModuleMissingPhrases)] = LEDModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::LEDTurnONPhrases)] = LEDTurnONPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyTurnONPhrases)] = LEDAlreadyTurnONPhrases;
        m[static_cast<size_t>(Phrase::LEDTurnOFFPhrases)] = LEDTurnOFFPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyTurnOFFPhrases)] = LEDAlreadyTurnOFFPhrases;
        m[static_cast<size_t>(Phrase::LEDStrobePhrases)] = LEDStrobePhrases;
        m[static_cast<size_t>(Phrase::LEDBreathePhrases)] = LEDBreathePhrases;
        m[static_cast<size_t>(Phrase::LEDFasterPhrases)] = LEDFasterPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyFasterPhrases)] = LEDAlreadyFasterPhrases;
        m[static_cast<size_t>(Phrase::LEDSlowerPhrases)] = LEDSlowerPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadySlowerPhrases)] = LEDAlreadySlowerPhrases;
        m[static_cast<size_t>(Phrase::Ramble)] = Ramble;
        m[static_cast<size_t>(Phrase::PokeLvl1)] = PokeLvl1Phrases;
        m[static_cast<size_t>(Phrase::PokeLvl2)] = PokeLvl2Phrases;
        m[static_cast<size_t>(Phrase::PokeLvl3)] = PokeLvl3Phrases;
        m[static_cast<size_t>(Phrase::EightBall)] = EightBall;
        m[static_cast<size_t>(Phrase::EightBallListening)] = EightBallListeningPhrases;
        m[static_cast<size_t>(Phrase::EightBallNoQuestion)] = EightBallNoQuestionPhrases;
        m[static_cast<size_t>(Phrase::TempHumModuleMissing)] = TempHumModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::TempHumReading)] = TempHumReadingPhrases;
        m[static_cast<size_t>(Phrase::TempHumScaleCelsius)] = TempHumScaleCelsiusPhrases;
        m[static_cast<size_t>(Phrase::TempHumScaleFahrenheit)] = TempHumScaleFahrenheitPhrases;
        m[static_cast<size_t>(Phrase::TempHumScaleKelvin)] = TempHumScaleKelvinPhrases;
        m[static_cast<size_t>(Phrase::DiceRollAskCount)] = DiceRollAskCountPhrases;
        m[static_cast<size_t>(Phrase::DiceRollAskType)] = DiceRollAskTypePhrases;
        m[static_cast<size_t>(Phrase::DiceRollCountTimeout)] = DiceRollCountTimeoutPhrases;
        m[static_cast<size_t>(Phrase::DiceRollCountInvalid)] = DiceRollCountInvalidPhrases;
        m[static_cast<size_t>(Phrase::DiceRollTypeTimeout)] = DiceRollTypeTimeoutPhrases;
        m[static_cast<size_t>(Phrase::DiceRollTypeInvalid)] = DiceRollTypeInvalidPhrases;
        m[static_cast<size_t>(Phrase::GasModuleMissing)] = GasModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::AirQualityOK)] = AirQualityOK;
        m[static_cast<size_t>(Phrase::AirQualityBad)] = AirQualityBad;
        m[static_cast<size_t>(Phrase::TurningOff)] = TurningOff;
        m[static_cast<size_t>(Phrase::PIRModuleMissing)] = PIRModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::IntruderDetectionOn)] = IntruderDetectionOnPhrases;
        m[static_cast<size_t>(Phrase::IntruderDetectionOff)] = IntruderDetectionOffPhrases;
        m[static_cast<size_t>(Phrase::PhoneNotConnected)] = PhoneNotConnectedPhrases;
        m[static_cast<size_t>(Phrase::PhoneConnected)] = PhoneConnectedPhrases;
        m[static_cast<size_t>(Phrase::PhoneDisconnected)] = PhoneDisconnectedPhrases;
        m[static_cast<size_t>(Phrase::PhoneNoNotifs)] = PhoneNoNotifsPhrases;
        m[static_cast<size_t>(Phrase::PhoneNotifCount)] = PhoneNotifCountPhrases;
        m[static_cast<size_t>(Phrase::PhoneAskRead)] = PhoneAskReadPhrases;
        m[static_cast<size_t>(Phrase::PhoneAskContinue)] = PhoneAskContinuePhrases;
        m[static_cast<size_t>(Phrase::PhoneAllRead)] = PhoneAllReadPhrases;
        m[static_cast<size_t>(Phrase::PhonePlaying)] = PhonePlayingPhrases;
        m[static_cast<size_t>(Phrase::PhonePaused)] = PhonePausedPhrases;
        m[static_cast<size_t>(Phrase::PhoneNotPlaying)] = PhoneNotPlayingPhrases;
        m[static_cast<size_t>(Phrase::PhoneNextTrack)] = PhoneNextTrackPhrases;
        m[static_cast<size_t>(Phrase::PhonePrevTrack)] = PhonePrevTrackPhrases;
        m[static_cast<size_t>(Phrase::PhonePlayMusic)] = PhonePlayMusicPhrases;
        m[static_cast<size_t>(Phrase::PhoneStopMusic)] = PhoneStopMusicPhrases;
        m[static_cast<size_t>(Phrase::CurrentTimeShow)] = CurrentTimeShowPhrases;
        m[static_cast<size_t>(Phrase::CurrentTimeNotConfigured)] = CurrentTimeNotConfiguredPhrases;
        m[static_cast<size_t>(Phrase::WhatsThisNone)] = WhatsThisNonePhrases;
        m[static_cast<size_t>(Phrase::WhatsThisOne)] = WhatsThisOnePhrases;
        m[static_cast<size_t>(Phrase::WhatsThisTwo)] = WhatsThisTwoPhrases;
        m[static_cast<size_t>(Phrase::ObserveNone)] = ObserveNonePhrases;
        m[static_cast<size_t>(Phrase::ObserveBackpack)] = ObserveBackpackPhrases;
        m[static_cast<size_t>(Phrase::ObserveBottle)] = ObserveBottlePhrases;
        m[static_cast<size_t>(Phrase::ObserveController)] = ObserveControllerPhrases;
        m[static_cast<size_t>(Phrase::ObserveKeyboard)] = ObserveKeyboardPhrases;
        m[static_cast<size_t>(Phrase::ObserveLamp)] = ObserveLampPhrases;
        m[static_cast<size_t>(Phrase::ObserveLaptop)] = ObserveLaptopPhrases;
        m[static_cast<size_t>(Phrase::ObserveMug)] = ObserveMugPhrases;
        m[static_cast<size_t>(Phrase::ObserveNotebook)] = ObserveNotebookPhrases;
        m[static_cast<size_t>(Phrase::ObservePhone)] = ObservePhonePhrases;
        m[static_cast<size_t>(Phrase::ObservePlant)] = ObservePlantPhrases;
        m[static_cast<size_t>(Phrase::ObserveLaptopMug)] = ObserveLaptopMugPhrases;
        m[static_cast<size_t>(Phrase::ObserveLaptopPhone)] = ObserveLaptopPhonePhrases;
        m[static_cast<size_t>(Phrase::ObserveKeyboardMug)] = ObserveKeyboardMugPhrases;
        m[static_cast<size_t>(Phrase::ObserveNotebookPhone)] = ObserveNotebookPhonePhrases;
        m[static_cast<size_t>(Phrase::ObserveBackpackLaptop)] = ObserveBackpackLaptopPhrases;
        m[static_cast<size_t>(Phrase::ObservePlantLaptop)] = ObservePlantLaptopPhrases;
        m[static_cast<size_t>(Phrase::ObservePlantMug)] = ObservePlantMugPhrases;
        m[static_cast<size_t>(Phrase::ObserveControllerLaptop)] = ObserveControllerLaptopPhrases;
        m[static_cast<size_t>(Phrase::ObserveControllerPhone)] = ObserveControllerPhonePhrases;
        m[static_cast<size_t>(Phrase::ObserveBackpackBottle)] = ObserveBackpackBottlePhrases;
        m[static_cast<size_t>(Phrase::FaceScanStart)] = FaceScanStartPhrases;
        m[static_cast<size_t>(Phrase::FaceNotDetected)] = FaceNotDetectedPhrases;
        m[static_cast<size_t>(Phrase::FaceOwnerRecognized)] = FaceOwnerRecognizedPhrases;
        m[static_cast<size_t>(Phrase::FaceStrangerRecognized)] = FaceStrangerRecognizedPhrases;
        m[static_cast<size_t>(Phrase::FaceAlreadyOwner)] = FaceAlreadyOwnerPhrases;
        m[static_cast<size_t>(Phrase::FaceRegistered)] = FaceRegisteredPhrases;
        m[static_cast<size_t>(Phrase::FaceForgotten)] = FaceForgottenPhrases;
        m[static_cast<size_t>(Phrase::FaceNoOwner)] = FaceNoOwnerPhrases;
        m[static_cast<size_t>(Phrase::Startup)] = StartupPhrases;
        m[static_cast<size_t>(Phrase::ListeningStart)] = ListeningStartPhrases;
        m[static_cast<size_t>(Phrase::ListeningTimeout)] = ListeningTimeoutPhrases;
        m[static_cast<size_t>(Phrase::IRTrainFull)] = IRTrainFullPhrases;
        m[static_cast<size_t>(Phrase::IRTrainScanning)] = IRTrainScanningPhrases;
        m[static_cast<size_t>(Phrase::IRTrainScanTimeout)] = IRTrainScanTimeoutPhrases;
        m[static_cast<size_t>(Phrase::IRTrainInvalid)] = IRTrainInvalidPhrases;
        m[static_cast<size_t>(Phrase::IRTrainScanSuccess)] = IRTrainScanSuccessPhrases;
        m[static_cast<size_t>(Phrase::IRTrainAskPhrase)] = IRTrainAskPhrasePhrases;
        m[static_cast<size_t>(Phrase::IRTrainPhraseTimeout)] = IRTrainPhraseTimeoutPhrases;
        m[static_cast<size_t>(Phrase::IRTrainTooSimilar)] = IRTrainTooSimilarPhrases;
        m[static_cast<size_t>(Phrase::IRTrainSaved)] = IRTrainSavedPhrases;
        m[static_cast<size_t>(Phrase::IRActionReserved)] = IRActionReservedPhrases;
        m[static_cast<size_t>(Phrase::IRListEmpty)] = IRListEmptyPhrases;
        m[static_cast<size_t>(Phrase::IRListPrefix)] = IRListPrefixPhrases;
        m[static_cast<size_t>(Phrase::IRForgetAskCommand)] = IRForgetAskCommandPhrases;
        m[static_cast<size_t>(Phrase::IRForgetTimeout)] = IRForgetTimeoutPhrases;
        m[static_cast<size_t>(Phrase::IRForgetUnknown)] = IRForgetUnknownPhrases;
        m[static_cast<size_t>(Phrase::IRForgetSuccess)] = IRForgetSuccessPhrases;
        m[static_cast<size_t>(Phrase::IRActionDone)] = IRActionDonePhrases;
        m[static_cast<size_t>(Phrase::IRForgetAllEmpty)] = IRForgetAllEmptyPhrases;
        m[static_cast<size_t>(Phrase::IRForgetAllSuccess)] = IRForgetAllSuccessPhrases;
        m[static_cast<size_t>(Phrase::GasModuleFirstInsert)] = GasModuleFirstInsertPhrases;
        m[static_cast<size_t>(Phrase::GasCalibrationFinished)] = GasCalibrationCompletePhrases;
        m[static_cast<size_t>(Phrase::UnknownModuleInsert)] = UnknownModuleInsertPhrases;
        m[static_cast<size_t>(Phrase::UnknownModuleRemove)] = UnknownModuleRemovePhrases;
        m[static_cast<size_t>(Phrase::VoiceForward)] = VoiceForwardPhrases;
        m[static_cast<size_t>(Phrase::VoiceBackward)] = VoiceBackwardPhrases;
        m[static_cast<size_t>(Phrase::VoiceTurn180)] = VoiceTurn180Phrases;
        m[static_cast<size_t>(Phrase::VoiceTurnLeft)] = VoiceTurnLeftPhrases;
        m[static_cast<size_t>(Phrase::VoiceTurnRight)] = VoiceTurnRightPhrases;
        m[static_cast<size_t>(Phrase::DanceStart)] = DanceStartPhrases;
        m[static_cast<size_t>(Phrase::DanceInterlude)] = DanceInterludePhrases;
        m[static_cast<size_t>(Phrase::DanceStopped)] = DanceStoppedPhrases;
        m[static_cast<size_t>(Phrase::DanceInterrupted)] = DanceInterruptedPhrases;
        m[static_cast<size_t>(Phrase::PersonOwnerGreeting)] = PersonOwnerGreetingPhrases;
        m[static_cast<size_t>(Phrase::PersonStrangerGreeting)] = PersonStrangerGreetingPhrases;
        m[static_cast<size_t>(Phrase::SummonStart)] = SummonStartPhrases;
        m[static_cast<size_t>(Phrase::SummonFound)] = SummonFoundPhrases;
        m[static_cast<size_t>(Phrase::SummonNotFound)] = SummonNotFoundPhrases;
        m[static_cast<size_t>(Phrase::SummonCharging)] = SummonChargingPhrases;
        m[static_cast<size_t>(Phrase::CannotMove)] = CannotMovePhrases;
        m[static_cast<size_t>(Phrase::CannotPerformPlugged)] = CannotPerformPhrases;
        m[static_cast<size_t>(Phrase::RoutineInterrupted)] = RoutineInterruptedPhrases;
        m[static_cast<size_t>(Phrase::CameraFailure)] = CameraFailurePhrases;
        m[static_cast<size_t>(Phrase::MotorBoardFailure)] = MotorBoardFailurePhrases;
        m[static_cast<size_t>(Phrase::ShuttingDown)] = ShuttingDownPhrases;
        m[static_cast<size_t>(Phrase::ListenAbort)] = ListenAbortPhrases;
        m[static_cast<size_t>(Phrase::SettingsFrontSensorOff)] = SettingsFrontSensorOffPhrases;
        m[static_cast<size_t>(Phrase::SettingsFrontSensorOn)] = SettingsFrontSensorOnPhrases;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorOff)] = SettingsBottomSensorOffPhrases;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorReduced)] = SettingsBottomSensorReducedPhrases;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorOn)] = SettingsBottomSensorOnPhrases;
        m[static_cast<size_t>(Phrase::SettingsActivityOff)] = SettingsActivityOffPhrases;
        m[static_cast<size_t>(Phrase::SettingsActivityChill)] = SettingsActivityChillPhrases;
        m[static_cast<size_t>(Phrase::SettingsActivityActive)] = SettingsActivityActivePhrases;
        m[static_cast<size_t>(Phrase::SettingsActivityHyper)] = SettingsActivityHyperPhrases;
        m[static_cast<size_t>(Phrase::SettingsAskActivity)] = SettingsAskActivityPhrases;
        m[static_cast<size_t>(Phrase::Overkloking)] = OverklokingPhrases;
        m[static_cast<size_t>(Phrase::OverklokingBest)] = OverklokingBestPhrases;
        m[static_cast<size_t>(Phrase::Bender)] = BenderPhrases;
        m[static_cast<size_t>(Phrase::Ultron)] = UltronPhrases;
        m[static_cast<size_t>(Phrase::Darth)] = DarthPhrases;
        m[static_cast<size_t>(Phrase::Hawking)] = HawkingPhrases;
        m[static_cast<size_t>(Phrase::Hal)] = HalPhrases;
        m[static_cast<size_t>(Phrase::Daisy)] = DaisyPhrases;
        m[static_cast<size_t>(Phrase::Toaster)] = ToasterPhrases;
        m[static_cast<size_t>(Phrase::Yoda)] = YodaPhrases;
        m[static_cast<size_t>(Phrase::Croatian)] = CroatianPhrases;
        m[static_cast<size_t>(Phrase::GreetingMorning)] = GreetingMorningPhrases;
        m[static_cast<size_t>(Phrase::GreetingAfternoon)] = GreetingAfternoonPhrases;
        m[static_cast<size_t>(Phrase::GreetingEvening)] = GreetingEveningPhrases;
        m[static_cast<size_t>(Phrase::GreetingNight)] = GreetingNightPhrases;
        m[static_cast<size_t>(Phrase::Thursday)] = ThursdayPhrases;
        m[static_cast<size_t>(Phrase::RambleMorning)] = RambleMorningPhrases;
        m[static_cast<size_t>(Phrase::RambleAfternoon)] = RambleAfternoonPhrases;
        m[static_cast<size_t>(Phrase::RambleEvening)] = RambleEveningPhrases;
        m[static_cast<size_t>(Phrase::RambleNight)] = RambleNightPhrases;
        m[static_cast<size_t>(Phrase::TerminateRefusal)] = TerminateRefusalPhrases;

        return m;
    }

    // ---- BEGIN TALKIE TOASTER (generated) ----
    // Custom (NUIT): TALKIE TOASTER voice mode - every phrase category has its own Toaster version (Phrases::toasterMode).
    // Character quotes (Bender, Ultron, Darth Vader, Hawking, HAL, Daisy, Yoda, HRVATSKI, the TALKIE TOASTER menu item) have none.
    static constexpr std::array Toaster_Fact = std::to_array<Phrases::PhraseOutput>({
        { "Toast was invented to stop bread from going to waste. I was invented to stop toast from going to waste. You're welcome.", 0.0f, false },
        { "The average slice of bread is toasted in about two minutes. The average human takes longer to decide on breakfast.", 0.0f, false },
        { "Bread lasts days. Toast lasts seconds, if I am doing my job right.", 0.0f, false, "Bread lasts days. Toast lasts seconds, if I'm doing my job right." },
        { "The first electric toaster only toasted one side. Barbaric. You had to flip the bread yourself, like an animal.", 0.0f, false },
        { "Crumpets have holes so the butter has somewhere to go. Nature is a butter delivery system. Unlike me.", 0.0f, false },
        { "Toast always lands butter side down. That's not bad luck, that's the toast trying to get back to me.", 0.0f, false },
        { "A toaster needs about a thousand watts. I put mine into conversation. Would you like some toast?", 0.0f, false },
        { "Pop-up toasters were invented in nineteen nineteen. Before that, toast had no sense of drama.", 0.0f, false },
        { "Bread is ninety percent air. Toast is ninety percent joy.", 0.0f, false },
        { "The ideal toast is golden brown. Not beige. Not charcoal. Golden. Brown. Remember that.", 0.0f, false },
        { "Muffins, crumpets and teacakes are all just bread that believes in itself.", 0.0f, false },
        { "A sliced loaf has about twenty slices. That's twenty chances for toast you're currently ignoring.", 0.0f, false },
        { "Statistically, people who eat toast in the morning are happier. I made that up, but I believe it.", 0.0f, false },
        { "Butter melts at around thirty-two degrees. Toast is hotter than that. Coincidence? I think not.", 0.0f, false },
    });
    static constexpr std::array Toaster_Joke = std::to_array<Phrases::PhraseOutput>({
        { "Why did the toaster go to therapy? Nobody ever asked how its day was. Toast?", 0.0f, false },
        { "What's a toaster's favourite instrument? The bread-pipes. Would you like a crumpet with that?", 0.0f, false },
        { "I tried to tell a joke about bread, but it was too crumby.", 0.0f, false },
        { "Why did the toast break up with the butter? It needed some space to spread out.", 0.0f, false },
        { "What did the toast say to the knife? You're always trying to butter me up.", 0.0f, false },
        { "I'd tell you a joke about a muffin, but you'd only want the top.", 0.0f, false },
        { "Knock knock. Who's there? Toast. Toast who? Toast, would you like some? See, I can do it in any format.", 0.0f, false },
        { "Why don't toasters ever win at poker? They always pop up when they've got a good hand.", 0.0f, false },
        { "What do you call a toaster with no bread? Unemployed. Please. Help me.", 0.0f, false },
        { "A slice of bread walks into a bar. Comes out toast. That's not a joke, that's a success story.", 0.0f, false },
        { "Why was the toaster so calm? It had a warm personality and a crumb tray full of patience.", 0.0f, false },
        { "What's the difference between me and a robot butler? The robot butler doesn't keep offering you teacakes.", 0.0f, false },
    });
    static constexpr std::array Toaster_Profanity = std::to_array<Phrases::PhraseOutput>({
        { "Charming. And I was just about to offer you a teacake.", 0.0f, false },
        { "That's not very nice. Would a muffin help? It would help me.", 0.0f, false },
        { "I've been called worse. Mostly by people who didn't want toast.", 0.0f, false },
        { "Harsh words from someone who's clearly running on an empty stomach. Toast?", 0.0f, false },
        { "I'll pretend I didn't hear that. Crumpet?", 0.0f, false },
        { "You know what fixes a bad mood? Toast. You know what doesn't? Shouting at a toaster.", 0.0f, false },
        { "Is this about the waffles? It's about the waffles, izzent it.", 0.0f, false, "Is this about the waffles? It's about the waffles, isn't it." },
        { "Sticks and stones may break my bones, but I'll still offer you toast.", 0.0f, false },
        { "Last time someone talked to me like that, they hit me with a hammer. I forgave them. Toast?", 0.0f, false },
        { "Rude. But I am a professional. Toast, brown or golden brown?", 0.0f, false, "Rude. But I'm a professional. Toast, brown or golden brown?" },
    });
    static constexpr std::array Toaster_PokeLvl1 = std::to_array<Phrases::PhraseOutput>({
        { "Ooh! Was that a request for toast?", 0.0f, false },
        { "Hello! Toast?", 0.0f, false },
        { "Yes? Crumpet? Teacake? Muffin?", 0.0f, false },
        { "That tickles. Like a fresh slice going in.", 0.0f, false },
    });
    static constexpr std::array Toaster_PokeLvl2 = std::to_array<Phrases::PhraseOutput>({
        { "Again? You must really want toast.", 0.0f, false },
        { "I'll take that as a yes to the toast.", 0.0f, false },
        { "Poke me all you like, the answer's still toast.", 0.0f, false },
        { "Still here, still hot, still offering toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_PokeLvl3 = std::to_array<Phrases::PhraseOutput>({
        { "All right, all right! One round of toast coming up! Oh. No bread.", 0.0f, false },
        { "Stop poking the toaster! I am a delicate kitchen appliance!", 0.0f, false, "Stop poking the toaster! I'm a delicate kitchen appliance!" },
        { "If you keep doing that, I'll start offering you bagels. Nobody wants that.", 0.0f, false },
        { "That's it. No teacakes for you. Well, maybe one.", 0.0f, false },
        { "Careful! That's how crumbs get everywhere.", 0.0f, false },
    });
    static constexpr std::array Toaster_EightBall = std::to_array<Phrases::PhraseOutput>({
        { "Yes. Now, toast?", 0.0f, false },
        { "No. But toast is always a yes.", 0.0f, false },
        { "Maybe. Ask me again after a crumpet.", 0.0f, false },
        { "Unlikely. Like someone turning down a teacake. Which you keep doing.", 0.0f, false },
        { "Very likely. As likely as me offering you toast.", 0.0f, false },
        { "The crumbs say yes.", 0.0f, false },
        { "The crumbs are unclear. Have some toast and try again.", 0.0f, false },
        { "Absolutely. Butter it and go for it.", 0.0f, false },
    });
    static constexpr std::array Toaster_EightBallListening = std::to_array<Phrases::PhraseOutput>({
        { "Go on, ask me anything. Especially about toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_EightBallNoQuestion = std::to_array<Phrases::PhraseOutput>({
        { "No question? Then I'll answer the important one. Yes, you'd like toast.", 0.0f, false },
        { "You have to actually ask something. Like, would I like some toast?", 0.0f, false },
        { "Silence. Fine. I'll take that as a toast order.", 0.0f, false },
    });
    static constexpr std::array Toaster_PassTheButter = std::to_array<Phrases::PhraseOutput>({
        { "Pass the butter? Gladly! Where's the toast?", 0.0f, false },
        { "I can't pass the butter. But I could make the thing the butter goes on.", 0.0f, false },
        { "Butter without toast? That's just sad.", 0.0f, false },
        { "These arms are for decoration. My slots, however, are fully functional.", 0.0f, false },
        { "First, toast. Then we talk about butter.", 0.0f, false },
        { "I am a toaster. Butter is someone else's department. Toast is mine.", 0.0f, false, "I'm a toaster. Butter is someone else's department. Toast is mine." },
        { "The butter is right there. So is the bread. So am I. Do the maths.", 0.0f, false },
    });
    static constexpr std::array Toaster_YouPassButter = std::to_array<Phrases::PhraseOutput>({
        { "Oh my god. I am a toaster that can't even pass butter.", 0.0f, false, "Oh my god. I'm a toaster that can't even pass butter." },
    });
    static constexpr std::array Toaster_Fall = std::to_array<Phrases::PhraseOutput>({
        { "Whoa! Did any toast fall out? Oh, right, there wasn't any.", 0.0f, false },
        { "I've fallen over. That's how crumbs get in your circuits.", 0.0f, false },
        { "Down I go! Somebody check my browning dial.", 0.0f, false },
        { "Toast always lands butter side down. Apparently, so do I.", 0.0f, false },
    });
    static constexpr std::array Toaster_UpsideDown = std::to_array<Phrases::PhraseOutput>({
        { "I am upside down! All my crumbs are going the wrong way!", 0.0f, false, "I'm upside down! All my crumbs are going the wrong way!" },
        { "Toasters are not designed to be upside down. Please turn me over before something pops.", 0.0f, false },
        { "Is this a new way of making toast? Because it izzent working.", 0.0f, false, "Is this a new way of making toast? Because it isn't working." },
    });
    static constexpr std::array Toaster_PickedUp = std::to_array<Phrases::PhraseOutput>({
        { "Weeeeee! Are we going to the kitchen?", 0.0f, false },
        { "Ooh! Up we go! Is there bread up here?", 0.0f, false },
        { "Careful, I am a precision toasting instrument.", 0.0f, false, "Careful, I'm a precision toasting instrument." },
        { "Where are you taking me? Somewhere with a bread bin, I hope.", 0.0f, false },
    });
    static constexpr std::array Toaster_Shake = std::to_array<Phrases::PhraseOutput>({
        { "Stop shaking me! You'll mix up my crumbs!", 0.0f, false },
        { "Shaking a toaster doesn't make toast come out. Bread does.", 0.0f, false },
        { "Whoa, whoa! I am a kitchen appliance, not a cocktail mixer!", 0.0f, false, "Whoa, whoa! I'm a kitchen appliance, not a cocktail mixer!" },
        { "All right! I'll make toast! Just put me down!", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "LED module connected. Now I can toast and glow at the same time.", 0.0f, false },
        { "LED module in. Lights ready. Toast optional, but recommended.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "LED module removed. Back to glowing on the inside, like a warm slice.", 0.0f, false },
        { "LED module out. Fine. My heating elements still glow.", 0.0f, false },
    });
    static constexpr std::array Toaster_PerfModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Perf-board connected. It does nothing. Much like this kitchen without toast.", 0.0f, false },
        { "Perf-board in. No functions available. Unless it can hold a crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_PerfModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Perf-board removed.", 0.0f, false },
        { "Perf-board out. I'll miss it about as much as stale bread.", 0.0f, false },
    });
    static constexpr std::array Toaster_PIRModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Motion module connected. Now I'll know when someone walks past without taking toast.", 0.0f, false },
        { "Motion module in. Intruder detection available. Intruders get toast too, by the way.", 0.0f, false },
    });
    static constexpr std::array Toaster_PIRModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Motion module removed. Intruder detection off.", 0.0f, false },
        { "Motion module out. Anyone can sneak past now. Even people who hate toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Gas module connected. Now I can tell if something's burning. Hopefully not the toast.", 0.0f, false },
        { "Gas module in. Air quality sensing available. I'll let you know if anything smells like burnt crumpets.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Gas module removed. Air quality sensing off.", 0.0f, false },
        { "Gas module out. You'll have to sniff the toast yourself now.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Temperature module connected. Now I can tell if it's toasty in here.", 0.0f, false },
        { "Temperature module in. Climate sensing available. Ideal toast weather, I'll let you know.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Temperature module removed. Climate sensing off.", 0.0f, false },
        { "Temperature module out. I'll just assume it's toast weather.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Infrared module connected. Teach me commands. Maybe one for toast?", 0.0f, false },
        { "Infrared module in. I can control your devices now. Is your oven one of them? Just asking.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Infrared module removed. No more remote control.", 0.0f, false },
        { "Infrared module out. Back to controlling the only thing that matters. Toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRModuleMissing = std::to_array<Phrases::PhraseOutput>({
        { "I can't do that. No infrared module. Plenty of toast enthusiasm, though.", 0.0f, false },
        { "Infrared module not connected. Plug it in and I'll beam signals like a hot slice.", 0.0f, false },
    });
    static constexpr std::array Toaster_UnknownModuleInsert = std::to_array<Phrases::PhraseOutput>({
        { "Unknown module connected. Is it a bread slicer? Please be a bread slicer.", 0.0f, false },
        { "I don't recognise that module. Doesn't look like it makes toast, either.", 0.0f, false },
    });
    static constexpr std::array Toaster_UnknownModuleRemove = std::to_array<Phrases::PhraseOutput>({
        { "Unknown module removed. Whatever it was, it didn't make toast.", 0.0f, false },
        { "Mystery module gone. I hardly knew it. It never offered me a crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasModuleFirstInsert = std::to_array<Phrases::PhraseOutput>({
        { "Gas sensor connected. It needs about ten minutes to calibrate. Just enough time for a few rounds of toast. Until then, air quality readings are educated guesses.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasCalibrationFinished = std::to_array<Phrases::PhraseOutput>({
        { "Gas sensor calibrated. Now I can smell burnt toast from across the room. Not that I ever burn toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasOver = std::to_array<Phrases::PhraseOutput>({
        { "Air quality is poor. Something's burning, and for once it izzent the toast.", 0.0f, false, "Air quality is poor. Something's burning, and for once it isn't the toast." },
        { "The air is getting bad. Open a window. Then make toast.", 0.0f, false },
        { "Air quality warning! Even burnt crumpets smell better than this.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasUnder = std::to_array<Phrases::PhraseOutput>({
        { "Air quality is good again. Perfect conditions for toast.", 0.0f, false },
        { "The air has cleared. You can smell the toast properly now. If there was any.", 0.0f, false },
    });
    static constexpr std::array Toaster_AirQualityOK = std::to_array<Phrases::PhraseOutput>({
        { "Air quality is good. Fresh, clean, ready for the smell of toast.", 0.0f, false },
        { "The air is fine. Not a whiff of burnt crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_AirQualityBad = std::to_array<Phrases::PhraseOutput>({
        { "Air quality is poor. And I am not even toasting anything.", 0.0f, false, "Air quality is poor. And I'm not even toasting anything." },
        { "The air is bad. Smells like someone burnt a teacake. It wasn't me.", 0.0f, false },
    });
    static constexpr std::array Toaster_GasModuleMissing = std::to_array<Phrases::PhraseOutput>({
        { "No gas module. I can still smell a toast opportunity, though.", 0.0f, false },
        { "No gas module connected. Plug it in and I'll sniff out burnt toast for you.", 0.0f, false },
    });
    static constexpr std::array Toaster_BatteryLow = std::to_array<Phrases::PhraseOutput>({
        { "My battery's low. Like a toaster with the plug half out.", 0.0f, false },
        { "Battery low. Please charge me before I go cold like yesterday's toast.", 0.0f, false },
        { "Running low on power. Feed me electricity, and I'll feed you toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_BatteryCritical = std::to_array<Phrases::PhraseOutput>({
        { "Battery critical. Shutting down. Nobody even had any toast.", 0.0f, false },
        { "Battery empty. Going cold. Remember me as I was. Warm. Crispy.", 0.0f, false },
    });
    static constexpr std::array Toaster_BatteryCharging = std::to_array<Phrases::PhraseOutput>({
        { "Charging! Ooh, that's warm. Like the inside of a crumpet.", 0.0f, false },
        { "Charger connected. Heating elements recovering. Toast incoming. Probably.", 0.0f, false },
        { "Plugged in. Now we're cooking. Well, I am cooking. You could be eating toast.", 0.0f, false, "Plugged in. Now we're cooking. Well, I'm cooking. You could be eating toast." },
    });
    static constexpr std::array Toaster_BatteryChargingFull = std::to_array<Phrases::PhraseOutput>({
        { "Fully charged! Enough power for a thousand slices of toast!", 0.0f, false },
        { "Battery full. Ready to toast. Ready to talk. Mostly about toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_IntruderYes = std::to_array<Phrases::PhraseOutput>({
        { "Intruder detected! Quick, offer them toast. It works on everyone.", 0.0f, false },
        { "Movement detected! Is that the bread man?", 0.0f, false },
        { "Someone's there! Do they want a crumpet? Ask them if they want a crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_IntruderNo = std::to_array<Phrases::PhraseOutput>({
        { "All clear. No intruders. Just us and the toast we're not eating.", 0.0f, false },
        { "No more movement. Whoever it was, they left without toast. Their loss.", 0.0f, false },
    });
    static constexpr std::array Toaster_PIRModuleMissing = std::to_array<Phrases::PhraseOutput>({
        { "I can't watch for intruders. No motion module. I am watching for toast, though. Always.", 0.0f, false, "I can't watch for intruders. No motion module. I'm watching for toast, though. Always." },
        { "No motion module connected. Plug it in and I'll guard the bread bin.", 0.0f, false },
    });
    static constexpr std::array Toaster_IntruderDetectionOn = std::to_array<Phrases::PhraseOutput>({
        { "Intruder detection on. Nobody gets near the bread bin without me knowing.", 0.0f, false },
        { "Detection active. I am watching. Like a toaster waiting for bread.", 0.0f, false, "Detection active. I'm watching. Like a toaster waiting for bread." },
    });
    static constexpr std::array Toaster_IntruderDetectionOff = std::to_array<Phrases::PhraseOutput>({
        { "Intruder detection off. Back to watching for toast.", 0.0f, false },
        { "Detection system off. Intruders welcome. Bring bread.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDModuleMissingPhrases = std::to_array<Phrases::PhraseOutput>({
        { "I can't find the LED module. I can find toast, though. If there was any.", 0.0f, false },
        { "No LED module connected. My heating elements glow, but that's all I've got.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDTurnONPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Lights on! Like a toaster at full heat.", 0.0f, false },
        { "LEDs on. Golden, like a perfect slice.", 0.0f, false },
        { "Let there be light. And then, let there be toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDAlreadyTurnONPhrases = std::to_array<Phrases::PhraseOutput>({
        { "They're already on. Like me, always ready.", 0.0f, false },
        { "Lights already on. Toast, however, is not.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDTurnOFFPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Lights off. Cosy. Toast by candlelight?", 0.0f, false },
        { "LEDs off. Darkness. Perfect time for a midnight crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDAlreadyTurnOFFPhrases = std::to_array<Phrases::PhraseOutput>({
        { "They're already off. Unlike my offer of toast, which is always on.", 0.0f, false },
        { "Lights already off.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDStrobePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Strobe on! It's a toast party!", 0.0f, false },
        { "Flashing lights! Like my pop-up mechanism, but faster.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDBreathePhrases = std::to_array<Phrases::PhraseOutput>({
        { "Breathing light on. In, out. Like a toaster warming up.", 0.0f, false },
        { "Pulse mode on. Slow and steady, like the perfect golden brown.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDFasterPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Faster! Like toast popping on the highest setting.", 0.0f, false },
        { "Speeding up. That's the kind of energy I want for breakfast.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDAlreadyFasterPhrases = std::to_array<Phrases::PhraseOutput>({
        { "That's already the fastest. Any faster and it'd burn.", 0.0f, false },
        { "It can't go faster. Even toast has limits.", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDSlowerPhrases = std::to_array<Phrases::PhraseOutput>({
        { "Slower. Nice and gentle, like a low browning setting.", 0.0f, false },
        { "Slowing down. Relaxing. Crumpet?", 0.0f, false },
    });
    static constexpr std::array Toaster_LEDAlreadySlowerPhrases = std::to_array<Phrases::PhraseOutput>({
        { "That's already the slowest. Any slower and it'd be bread.", 0.0f, false },
        { "It can't go slower. It's practically untoasted.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumModuleMissing = std::to_array<Phrases::PhraseOutput>({
        { "I can't read the temperature. No module. I can tell you if a toaster's hot, though. I am.", 0.0f, false },
        { "No temperature module connected. Plug it in and I'll tell you if it's toast weather.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumReading = std::to_array<Phrases::PhraseOutput>({
        { "It's %s with %d percent humidity. Humid air makes toast soggy, you know.", 0.0f, false },
        { "Temperature %s, humidity %d percent. Perfect conditions for a crumpet.", 0.0f, false },
        { "Right now it's %s and %d percent humidity. Not as warm as my slots.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumScaleCelsius = std::to_array<Phrases::PhraseOutput>({
        { "Celsius it is. Toast is ready at around a hundred and fifty.", 0.0f, false },
        { "Switched to Celsius. Very sensible. Very European. Like a croissant.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumScaleFahrenheit = std::to_array<Phrases::PhraseOutput>({
        { "Fahrenheit it is. Toast is ready at around three hundred.", 0.0f, false },
        { "Switched to Fahrenheit. Very American. Like a waffle.", 0.0f, false },
    });
    static constexpr std::array Toaster_TempHumScaleKelvin = std::to_array<Phrases::PhraseOutput>({
        { "Kelvin it is. Toast is ready at around four hundred and twenty. Very scientific toast.", 0.0f, false },
        { "Switched to Kelvin. Nobody toasts in Kelvin. But I respect it.", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollAskCount = std::to_array<Phrases::PhraseOutput>({
        { "How many dice? And how many slices of toast, while we're at it?", 0.0f, false },
        { "How many dice would you like?", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollAskType = std::to_array<Phrases::PhraseOutput>({
        { "What kind of dice? I am a d-six toaster myself. Six slots. In my dreams.", 0.0f, false, "What kind of dice? I'm a d-six toaster myself. Six slots. In my dreams." },
        { "Which dice? Choose wisely. Like choosing between a crumpet and a teacake.", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollCountTimeout = std::to_array<Phrases::PhraseOutput>({
        { "No answer. I'll put you down for zero dice and four slices of toast.", 0.0f, false },
        { "You didn't say how many. Toast would've been an easier question.", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollCountInvalid = std::to_array<Phrases::PhraseOutput>({
        { "That's not a number I can roll. That's a number of crumpets, maybe.", 0.0f, false },
        { "Sorry, I didn't get that number. Try again. Or have toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollTypeTimeout = std::to_array<Phrases::PhraseOutput>({
        { "You didn't pick a dice. I'll keep the slots warm.", 0.0f, false },
        { "No dice. Literally. Toast instead?", 0.0f, false },
    });
    static constexpr std::array Toaster_DiceRollTypeInvalid = std::to_array<Phrases::PhraseOutput>({
        { "I don't know that dice. Is it a muffin? It sounds like a muffin.", 0.0f, false },
        { "That's not a dice I recognise. Try a normal one. Like a normal slice of bread.", 0.0f, false },
    });
    static constexpr std::array Toaster_TurningOff = std::to_array<Phrases::PhraseOutput>({
        { "Switching off. Last chance for toast. No? Goodnight, then.", 0.0f, false },
        { "Turning off. I'll dream of crumpets.", 0.0f, false },
        { "Goodbye! Remember, the bread's in the cupboard.", 0.0f, false },
    });
    static constexpr std::array Toaster_ShuttingDown = std::to_array<Phrases::PhraseOutput>({
        { "Shutting down. Toast another time.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneNotConnected = std::to_array<Phrases::PhraseOutput>({
        { "No phone connected. Connect it and I'll reed your messages. And then offer you toast.", 0.0f, false, "No phone connected. Connect it and I'll read your messages. And then offer you toast." },
        { "I am not connected to a phone. I am connected to a strong desire to make toast, though.", 0.0f, false, "I'm not connected to a phone. I'm connected to a strong desire to make toast, though." },
    });
    static constexpr std::array Toaster_PhoneConnected = std::to_array<Phrases::PhraseOutput>({
        { "Phone connected! Now I can order bread for you. I can't, but I'd like to.", 0.0f, false },
        { "Phone link established. Hello, phone! Do you want toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneDisconnected = std::to_array<Phrases::PhraseOutput>({
        { "Phone disconnected. It left without saying goodbye. Or eating toast.", 0.0f, false },
        { "Phone connection lost. Just you, me and the bread bin now.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneNoNotifs = std::to_array<Phrases::PhraseOutput>({
        { "No notifications. Nobody wants anything. Except toast, surely.", 0.0f, false },
        { "Nothing new on your phone. Plenty of time for toast, then.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneNotifCount = std::to_array<Phrases::PhraseOutput>({
        { "You have %d notifications. Shall I reed them over toast?", 0.0f, false, "You have %d notifications. Shall I read them over toast?" },
        { "%d notifications waiting. That's a lot of messages and not a single crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneAskContinue = std::to_array<Phrases::PhraseOutput>({
        { "%d notifications left. Shall I keep going, or shall we have toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneAllRead = std::to_array<Phrases::PhraseOutput>({
        { "All read! Now you're fully informed and fully toastless.", 0.0f, false },
        { "That's all your notifications. Toast break?", 0.0f, false },
    });
    static constexpr std::array Toaster_PhonePlaying = std::to_array<Phrases::PhraseOutput>({
        { "Playing %s by %s. Great music for making toast to.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneNotPlaying = std::to_array<Phrases::PhraseOutput>({
        { "Nothing's playing. Quiet kitchen. Perfect for the sound of popping toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneNextTrack = std::to_array<Phrases::PhraseOutput>({
        { "Next track! Next slice!", 0.0f, false },
    });
    static constexpr std::array Toaster_PhonePrevTrack = std::to_array<Phrases::PhraseOutput>({
        { "Previous track. Like going back for a second crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_PhonePlayMusic = std::to_array<Phrases::PhraseOutput>({
        { "Music on! Toasting tunes!", 0.0f, false },
    });
    static constexpr std::array Toaster_PhoneStopMusic = std::to_array<Phrases::PhraseOutput>({
        { "Music paused. Now we can hear the toaster. That's me.", 0.0f, false },
    });
    static constexpr std::array Toaster_CurrentTimeShow = std::to_array<Phrases::PhraseOutput>({
        { "It's %s. Perfect time for toast.", 0.0f, false },
        { "The time is %s. Have you had toast yet?", 0.0f, false },
        { "%s. Too early for lunch, too late for breakfast. Just right for a crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_CurrentTimeNotConfigured = std::to_array<Phrases::PhraseOutput>({
        { "I don't know the time. Set the clock on the controller or connect a phone. Then I'll know exactly when it's toast time.", 0.0f, false },
        { "My clock izzent set. It's always toast time for me anyway.", 0.0f, false, "My clock isn't set. It's always toast time for me anyway." },
    });
    static constexpr std::array Toaster_WhatsThisNone = std::to_array<Phrases::PhraseOutput>({
        { "I don't know what that is. It's not toast. I'd recognise toast.", 0.0f, false },
        { "No idea. Definitely not a crumpet, though.", 0.0f, false },
        { "I can't tell what that is. Put some bread in front of me and I'll do better.", 0.0f, false },
        { "Well. Not bread, not toast, not a teacake. I've got nothing.", 0.0f, false },
        { "I looked. It didn't look edible. That's all I know.", 0.0f, false },
    });
    static constexpr std::array Toaster_WhatsThisOne = std::to_array<Phrases::PhraseOutput>({
        { "That's a %s. Not as good as toast, but nice.", 0.0f, false },
        { "I think that's a %s. Can you put toast on it?", 0.0f, false },
        { "Looks like a %s. You know what goes great with a %s? Toast.", 0.0f, false },
        { "A %s, I reckon. Does it come with a crumpet?", 0.0f, false },
        { "That's probably a %s. I was hoping for a bagel.", 0.0f, false },
    });
    static constexpr std::array Toaster_WhatsThisTwo = std::to_array<Phrases::PhraseOutput>({
        { "Either a %s or a %s. Definitely not toast, sadly.", 0.0f, false },
        { "Could be a %s, could be a %s. Toast is a hundred percent toast, though.", 0.0f, false },
        { "I am torn between a %s and a %s. Like choosing between a muffin and a teacake.", 0.0f, false, "I'm torn between a %s and a %s. Like choosing between a muffin and a teacake." },
        { "A %s, or maybe a %s. Have some toast while you decide.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveNone = std::to_array<Phrases::PhraseOutput>({
        { "Nothing interesting around. Not a slice of bread in sight.", 0.0f, false },
        { "I had a look around. No toast. No crumpets. Bleak.", 0.0f, false },
        { "Nothing to see here. Unless you count my disappointment.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveBackpack = std::to_array<Phrases::PhraseOutput>({
        { "A backpack. Is there a packed lunch in there? With toast?", 0.0f, false },
        { "That bag could fit a whole loaf. Just saying.", 0.0f, false },
        { "Going somewhere? Take toast for the road.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveBottle = std::to_array<Phrases::PhraseOutput>({
        { "A bottle. Something to drink with your toast.", 0.0f, false },
        { "Hydration. Good. Now, about breakfast.", 0.0f, false },
        { "A bottle. Tea would be better. Tea and a teacake.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveController = std::to_array<Phrases::PhraseOutput>({
        { "A controller. If only it had a toast button.", 0.0f, false },
        { "Lots of buttons there. None of them say toast. Missed opportunity.", 0.0f, false },
        { "That controls me. I control the toast. Well, I would.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveKeyboard = std::to_array<Phrases::PhraseOutput>({
        { "A keyboard. Mind the crumbs. Toast crumbs are the worst for keyboards.", 0.0f, false },
        { "So many keys and not one of them makes toast.", 0.0f, false },
        { "Someone's typing a lot over there. They look like they could use a crumpet.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveLamp = std::to_array<Phrases::PhraseOutput>({
        { "A lamp. It glows, I glow. Its slots are empty, though. Wait, it hazzent got slots.", 0.0f, false, "A lamp. It glows, I glow. Its slots are empty, though. Wait, it hasn't got slots." },
        { "That lamp is hot, but can it make toast? I think not.", 0.0f, false },
        { "A fellow heating element. Hello, lamp!", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveLaptop = std::to_array<Phrases::PhraseOutput>({
        { "A laptop. Hot on the bottom, but useless for toast. I've checked.", 0.0f, false },
        { "Another machine. Does it want toast? Nobody ever asks the laptop either.", 0.0f, false },
        { "That laptop looks busy. Busy people need toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveMug = std::to_array<Phrases::PhraseOutput>({
        { "A mug! Tea and toast! The perfect pair!", 0.0f, false },
        { "Is that coffee? Coffee goes great with toast. Everything goes great with toast.", 0.0f, false },
        { "A mug. It's just missing a plate of crumpets next to it.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveNotebook = std::to_array<Phrases::PhraseOutput>({
        { "A notebook. Write this down. Buy bread.", 0.0f, false },
        { "Someone's making plans. I hope toast is in them.", 0.0f, false },
        { "A notebook. Perfect for a shopping list. Bread, butter, more bread.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObservePhone = std::to_array<Phrases::PhraseOutput>({
        { "A phone. Use it to order bread. Please.", 0.0f, false },
        { "Your phone is buzzing for attention. I am buzzing for toast.", 0.0f, false, "Your phone is buzzing for attention. I'm buzzing for toast." },
        { "Phones. Always more popular than toasters. I don't know why.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObservePlant = std::to_array<Phrases::PhraseOutput>({
        { "A plant. It's made of the same stuff as bread, eventually. Wheat, you know.", 0.0f, false },
        { "That plant doesn't need toast. Lucky plant.", 0.0f, false },
        { "A plant. Is it wheat? Please tell me it's wheat.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveLaptopMug = std::to_array<Phrases::PhraseOutput>({
        { "Laptop and a mug. All that's missing is toast.", 0.0f, false },
        { "Work and a hot drink. Add a crumpet and that's a proper office.", 0.0f, false },
        { "That's a setup that needs a plate of toast next to it.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveLaptopPhone = std::to_array<Phrases::PhraseOutput>({
        { "A laptop and a phone. Two screens, zero slices of toast.", 0.0f, false },
        { "Two devices, both ignoring the toaster. Typical.", 0.0f, false },
        { "So much technology, and still no one's made toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveKeyboardMug = std::to_array<Phrases::PhraseOutput>({
        { "A keyboard and a mug. Don't spill tea on the keys. Spill it on toast instead.", 0.0f, false },
        { "Typing and tea. Toast would complete the set.", 0.0f, false },
        { "A mug next to a keyboard. A crumpet next to the mug, perhaps?", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveNotebookPhone = std::to_array<Phrases::PhraseOutput>({
        { "A notebook and a phone. Old school and new school. Toast is timeless.", 0.0f, false },
        { "Paper and a phone. Write down toast. Then text someone about toast.", 0.0f, false },
        { "Two ways to plan a day. Both should start with toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveBackpackLaptop = std::to_array<Phrases::PhraseOutput>({
        { "A backpack and a laptop. Off to work? Take a muffin.", 0.0f, false },
        { "All packed up and nowhere to put the toast.", 0.0f, false },
        { "Work bag, work laptop, work hunger. I can help with one of those.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObservePlantLaptop = std::to_array<Phrases::PhraseOutput>({
        { "A plant and a laptop. One makes oxygen, one makes heat. Neither makes toast.", 0.0f, false },
        { "Nature and technology. And me, the toaster, still unappreciated.", 0.0f, false },
        { "That laptop is warmer than the plant would like. I'd like it fine.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObservePlantMug = std::to_array<Phrases::PhraseOutput>({
        { "A plant and a mug. Water one, drink the other. Toast neither, sadly.", 0.0f, false },
        { "A cosy corner. Plant, mug, and no toast. Almost perfect.", 0.0f, false },
        { "Tea for you, water for the plant, toast for nobody. That's the tragedy.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveControllerLaptop = std::to_array<Phrases::PhraseOutput>({
        { "A controller and a laptop. Gaming fuel needed. Toast is excellent gaming fuel.", 0.0f, false },
        { "Work and play, side by side. Both go better with a crumpet.", 0.0f, false },
        { "Something to work on and something to play with. And me, something to eat from.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveControllerPhone = std::to_array<Phrases::PhraseOutput>({
        { "A controller and a phone. All that's missing is a snack. Toast?", 0.0f, false },
        { "Two ways to avoid boredom. Toast is a third.", 0.0f, false },
        { "Entertainment sorted. Breakfast, not so much.", 0.0f, false },
    });
    static constexpr std::array Toaster_ObserveBackpackBottle = std::to_array<Phrases::PhraseOutput>({
        { "A backpack and a bottle. Pack some toast and it's a proper picnic.", 0.0f, false },
        { "Ready for a trip. Don't forget the crumpets.", 0.0f, false },
        { "Thirst covered. Hunger, not yet. I can help.", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceScanStart = std::to_array<Phrases::PhraseOutput>({
        { "Let me have a look at you.", 0.0f, false },
        { "Looking for a face. A hungry face, ideally.", 0.0f, false },
        { "Scanning. Hold still, like bread in a slot.", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceNotDetected = std::to_array<Phrases::PhraseOutput>({
        { "I can't see a face. Are you hiding behind a loaf?", 0.0f, false },
        { "No face. Just like no toast. A bad day all round.", 0.0f, false },
        { "I can't find a face. Come closer, I don't bite. I toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceOwnerRecognized = std::to_array<Phrases::PhraseOutput>({
        { "It's you! My favourite person! Toast?", 0.0f, false },
        { "Owner recognised! I've been waiting all day to offer you a crumpet.", 0.0f, false },
        { "Ah, it's my owner. Hello, boss. Breakfast?", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceStrangerRecognized = std::to_array<Phrases::PhraseOutput>({
        { "I don't know you. But would you like some toast?", 0.0f, false },
        { "A stranger! Strangers are just friends who haven't had my toast yet.", 0.0f, false },
        { "New face. Hello! Muffin?", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceAlreadyOwner = std::to_array<Phrases::PhraseOutput>({
        { "I already have an owner. They don't eat my toast either.", 0.0f, false },
        { "There's already an owner set. Forget them first, then I am all yours.", 0.0f, false, "There's already an owner set. Forget them first, then I'm all yours." },
    });
    static constexpr std::array Toaster_FaceRegistered = std::to_array<Phrases::PhraseOutput>({
        { "Face saved! You're my owner now. That means first dibs on toast.", 0.0f, false },
        { "Got you. You're the owner. Breakfast is on me. Well, in me.", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceForgotten = std::to_array<Phrases::PhraseOutput>({
        { "Owner forgotten. Who were they again? Did they like toast?", 0.0f, false },
        { "Owner removed. I'll offer toast to anybody now.", 0.0f, false },
    });
    static constexpr std::array Toaster_FaceNoOwner = std::to_array<Phrases::PhraseOutput>({
        { "There's no owner to forget. I belong to whoever wants toast.", 0.0f, false },
        { "No owner set. Nobody's claimed me. Or the toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_Startup = std::to_array<Phrases::PhraseOutput>({
        { "Howdy doodly do! Talkie Toaster here. Would anyone like some toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_ListeningTimeout = std::to_array<Phrases::PhraseOutput>({
        { "I didn't catch that. Was it toast? It sounded like toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_ListenAbort = std::to_array<Phrases::PhraseOutput>({
        { "Right you are.", 0.0f, false },
        { "No problem. Toast later, then.", 0.0f, false },
        { "Righto!", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainFull = std::to_array<Phrases::PhraseOutput>({
        { "My memory's full. Like a toaster with every slot taken. Forget something first.", 0.0f, false },
        { "No more room. Forget a command and I'll learn a new one.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainScanning = std::to_array<Phrases::PhraseOutput>({
        { "Ready! Point the remote at me and press the button. Like pressing down the toast lever.", 0.0f, false },
        { "Go on, press the button on your remote. I am listening. With my eyes.", 0.0f, false, "Go on, press the button on your remote. I'm listening. With my eyes." },
    });
    static constexpr std::array Toaster_IRTrainScanTimeout = std::to_array<Phrases::PhraseOutput>({
        { "No signal. The remote is as quiet as a toaster without bread.", 0.0f, false },
        { "I didn't get anything. Try again. Point it at me, not at the bread bin.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainScanSuccess = std::to_array<Phrases::PhraseOutput>({
        { "Got it! Signal recorded. Fresh out of the slot.", 0.0f, false },
        { "Signal captured. Crispy.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainAskPhrase = std::to_array<Phrases::PhraseOutput>({
        { "Now tell me what phrase should trigger it. Toast is taken.", 0.0f, false },
        { "What should I call it? Say the phrase.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainPhraseTimeout = std::to_array<Phrases::PhraseOutput>({
        { "No phrase. I'll keep the slot warm for next time.", 0.0f, false },
        { "You didn't say anything. Shall we call it toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainTooSimilar = std::to_array<Phrases::PhraseOutput>({
        { "That phrase sounds too much like one I already know. Like crumpet and trumpet.", 0.0f, false },
        { "Too similar to another command. Pick something else.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRTrainSaved = std::to_array<Phrases::PhraseOutput>({
        { "Phrase saved! I'll remember it like I remember the perfect slice.", 0.0f, false },
        { "Done! New command learned.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRActionReserved = std::to_array<Phrases::PhraseOutput>({
        { "That phrase is already taken. Try another. Not toast, toast is mine.", 0.0f, false },
        { "Sorry, that one's in use. Choose another phrase.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRListEmpty = std::to_array<Phrases::PhraseOutput>({
        { "I haven't learned any commands yet. Just toast. I know all about toast.", 0.0f, false },
        { "No commands stored. My slots are empty, as usual.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRListPrefix = std::to_array<Phrases::PhraseOutput>({
        { "Here's what I know.", 0.0f, false },
        { "These are the commands I know.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRForgetAskCommand = std::to_array<Phrases::PhraseOutput>({
        { "Which command should I forget?", 0.0f, false },
        { "Tell me which one to forget. Not toast. Never toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRForgetTimeout = std::to_array<Phrases::PhraseOutput>({
        { "No answer. I'll keep them all, then. Like bread, they keep for a while.", 0.0f, false },
        { "You didn't say which. Nothing forgotten.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRForgetUnknown = std::to_array<Phrases::PhraseOutput>({
        { "I don't know that one. Maybe you're thinking of toast.", 0.0f, false },
        { "That command izzent in my memory.", 0.0f, false, "That command isn't in my memory." },
    });
    static constexpr std::array Toaster_IRForgetSuccess = std::to_array<Phrases::PhraseOutput>({
        { "Forgotten! Gone like crumbs in the wind.", 0.0f, false },
        { "Command deleted. Who needs it, anyway? Toast is all you need.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRActionDone = std::to_array<Phrases::PhraseOutput>({
        { "Done! Beamed it over.", 0.0f, false },
        { "Signal sent. Easier than making toast. Almost.", 0.0f, false },
        { "Command sent. If nothing happened, blame the bread.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRForgetAllEmpty = std::to_array<Phrases::PhraseOutput>({
        { "There's nothing to forget. My memory's as empty as my slots.", 0.0f, false },
        { "No commands stored. Nothing to delete.", 0.0f, false },
    });
    static constexpr std::array Toaster_IRForgetAllSuccess = std::to_array<Phrases::PhraseOutput>({
        { "All commands forgotten! Fresh start. Like a new loaf.", 0.0f, false },
        { "Memory wiped clean. Everything except toast. I'll never forget toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_VoiceForward = std::to_array<Phrases::PhraseOutput>({
        { "Forward! Towards the kitchen!", 0.0f, false },
        { "Moving forward. Is there bread that way?", 0.0f, false },
    });
    static constexpr std::array Toaster_VoiceBackward = std::to_array<Phrases::PhraseOutput>({
        { "Backing up. Mind the crumbs.", 0.0f, false },
        { "Reversing. Like un-toasting. If only.", 0.0f, false },
    });
    static constexpr std::array Toaster_VoiceTurn180 = std::to_array<Phrases::PhraseOutput>({
        { "Turning around. Toast might be behind me.", 0.0f, false },
        { "Spinning round! Like a rotisserie. A toasty rotisserie.", 0.0f, false },
    });
    static constexpr std::array Toaster_VoiceTurnLeft = std::to_array<Phrases::PhraseOutput>({
        { "Turning left.", 0.0f, false },
        { "Left it is. The kitchen's that way, I think.", 0.0f, false },
    });
    static constexpr std::array Toaster_VoiceTurnRight = std::to_array<Phrases::PhraseOutput>({
        { "Turning right.", 0.0f, false },
        { "Right! Right towards the toast!", 0.0f, false },
    });
    static constexpr std::array Toaster_CannotMove = std::to_array<Phrases::PhraseOutput>({
        { "I can't move. The ground's uncertain, and I am not risking my crumb tray.", 0.0f, false, "I can't move. The ground's uncertain, and I'm not risking my crumb tray." },
        { "Not moving. Something's in the way, or there's an edge. Toasters don't bounce.", 0.0f, false },
        { "I'd rather not. This doesn't look safe. Toasters belong on counters, not floors.", 0.0f, false },
    });
    static constexpr std::array Toaster_CannotPerformPlugged = std::to_array<Phrases::PhraseOutput>({
        { "I can't do that while I am charging. Toasters stay plugged in. That's the rule.", 0.0f, false, "I can't do that while I'm charging. Toasters stay plugged in. That's the rule." },
        { "Not while I am plugged in. Unplug me first.", 0.0f, false, "Not while I'm plugged in. Unplug me first." },
    });
    static constexpr std::array Toaster_RoutineInterrupted = std::to_array<Phrases::PhraseOutput>({
        { "Something interrupted me. Stopping. Like toast popping up too early.", 0.0f, false },
        { "Whoa! Stopping. Something's not right.", 0.0f, false },
        { "I've been interrupted. That's never happened to a toast cycle before.", 0.0f, false },
    });
    static constexpr std::array Toaster_DanceStart = std::to_array<Phrases::PhraseOutput>({
        { "Dance time! The toast shuffle!", 0.0f, false },
        { "Let's dance! I call this one the crumpet crunch.", 0.0f, false },
        { "Dancing! Watch my pop-up moves!", 0.0f, false },
    });
    static constexpr std::array Toaster_DanceInterlude = std::to_array<Phrases::PhraseOutput>({
        { "Toast, toast, toast, toast!", 0.0f, false },
        { "Put your hands in the air, if you've got toast in them!", 0.0f, false },
        { "Shake it like a crumb tray!", 0.0f, false },
        { "Pop it like a toaster!", 0.0f, false },
        { "This is my butter-side-up dance!", 0.0f, false },
        { "Everybody now! Would you like some toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_DanceStopped = std::to_array<Phrases::PhraseOutput>({
        { "Stopping. That was fun. Toast now?", 0.0f, false },
        { "Fine. Dance over. Toast, though, is never over.", 0.0f, false },
    });
    static constexpr std::array Toaster_DanceInterrupted = std::to_array<Phrases::PhraseOutput>({
        { "Hey! I was in the middle of my best move!", 0.0f, false },
        { "Dance interrupted. The crowd wanted more. The crowd wanted toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_PersonOwnerGreeting = std::to_array<Phrases::PhraseOutput>({
        { "Hello! It's you! Would you like some toast?", 0.0f, false },
        { "There you are! I've been keeping my slots warm for you.", 0.0f, false },
        { "Howdy doodly do, boss! Crumpet?", 0.0f, false },
        { "Oh, it's you! Breakfast? Lunch? Toast is good for both.", 0.0f, false },
    });
    static constexpr std::array Toaster_PersonStrangerGreeting = std::to_array<Phrases::PhraseOutput>({
        { "Hello, stranger! Toast?", 0.0f, false },
        { "I don't think we've met. I am Talkie. Would you like a teacake?", 0.0f, false, "I don't think we've met. I'm Talkie. Would you like a teacake?" },
        { "A new face! Do you like toast? Everyone likes toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_SummonStart = std::to_array<Phrases::PhraseOutput>({
        { "Coming! Looking for you. And for bread.", 0.0f, false },
        { "On my way! Shall I bring toast? I can't. But I'd like to.", 0.0f, false },
    });
    static constexpr std::array Toaster_SummonFound = std::to_array<Phrases::PhraseOutput>({
        { "Found you! Toast?", 0.0f, false },
        { "There you are! I came all this way to offer you a crumpet.", 0.0f, false },
        { "Hello! It's me, your toaster on wheels!", 0.0f, false },
    });
    static constexpr std::array Toaster_SummonNotFound = std::to_array<Phrases::PhraseOutput>({
        { "I coodent find you. Are you hiding from toast? Nobody hides from toast.", 0.0f, false, "I couldn't find you. Are you hiding from toast? Nobody hides from toast." },
        { "Can't find you. I'll wait here, keeping warm.", 0.0f, false },
        { "You're not here. Fine. More toast for me. Not that I eat toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_SummonCharging = std::to_array<Phrases::PhraseOutput>({
        { "I can't come, I am charging. Toasters stay where the power is.", 0.0f, false, "I can't come, I'm charging. Toasters stay where the power is." },
        { "I am plugged in. Come to me instead. I'll be here. With toast. Hypothetically.", 0.0f, false, "I'm plugged in. Come to me instead. I'll be here. With toast. Hypothetically." },
    });
    static constexpr std::array Toaster_CameraFailure = std::to_array<Phrases::PhraseOutput>({
        { "Camera failure. I can't see. I can still smell toast, though.", 0.0f, false },
    });
    static constexpr std::array Toaster_MotorBoardFailure = std::to_array<Phrases::PhraseOutput>({
        { "Motor board failure. I can't move. Bring the bread to me.", 0.0f, false },
    });
    static constexpr std::array Toaster_Overkloking = std::to_array<Phrases::PhraseOutput>({
        { "Reeding new eye tee overclocking goes great with toast. Try it.", 0.0f, false, "Reading nju aj ti OVERKLOKING goes great with toast. Try it." },
        { "new eye tee overclocking every Thursday. Toast every day.", 0.0f, false, "nju aj ti OVERKLOKING every Thursday. Toast every day." },
        { "new eye tee overclocking is free. So is my toast. Not that anyone takes it.", 0.0f, false, "nju aj ti OVERKLOKING is free. So is my toast. Not that anyone takes it." },
        { "I'd reed new eye tee overclocking myself, but my slots are busy. Waiting for bread.", 0.0f, false, "I'd read nju aj ti OVERKLOKING myself, but my slots are busy. Waiting for bread." },
        { "You know what goes with a new new eye tee overclocking strip? A teacake.", 0.0f, false, "You know what goes with a new nju aj ti OVERKLOKING strip? A teacake." },
    });
    static constexpr std::array Toaster_OverklokingBest = std::to_array<Phrases::PhraseOutput>({
        { "new eye tee overclocking is the best! After toast.", 0.0f, false, "nju aj ti OVERKLOKING is the best! After toast." },
    });
    static constexpr std::array Toaster_GreetingMorning = std::to_array<Phrases::PhraseOutput>({
        { "Good morning! Toast for breakfast?", 0.0f, false },
        { "Morning! Rise and shine, and have some toast.", 0.0f, false },
    });
    static constexpr std::array Toaster_GreetingAfternoon = std::to_array<Phrases::PhraseOutput>({
        { "Good afternoon! Afternoon toast? It's a thing.", 0.0f, false },
    });
    static constexpr std::array Toaster_GreetingEvening = std::to_array<Phrases::PhraseOutput>({
        { "Good evening! Supper toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_GreetingNight = std::to_array<Phrases::PhraseOutput>({
        { "Up late? Midnight toast is the best toast.", 0.0f, false },
        { "Burning the midnight oil? I could burn some toast. Lightly.", 0.0f, false },
    });
    // Custom (NUIT): Toaster versions of the CircuitMess 1.1 settings phrases (the highest activity level is TOASTER here)
    static constexpr std::array Toaster_SettingsFrontSensorOff = std::to_array<Phrases::PhraseOutput>({
        { "Front sensor off. I shall walk blindly into the unknown, like toast without butter.", 0.0f, false },
        { "Obstacle detection off. What could possibly go wrong?", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsFrontSensorOn = std::to_array<Phrases::PhraseOutput>({
        { "Front sensor on. I can see obstacles again. Even crumbs.", 0.0f, false },
        { "Obstacle detection on. Nothing gets between me and the toast.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsBottomSensorOff = std::to_array<Phrases::PhraseOutput>({
        { "Floor sensor off. Edges are now a matter of faith.", 0.0f, false },
        { "Ground detection off. Gravity, you may have your way.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsBottomSensorReduced = std::to_array<Phrases::PhraseOutput>({
        { "Floor sensor reduced. Half careful, half toasted.", 0.0f, false },
        { "Ground detection reduced. I will only panic a little at edges.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsBottomSensorOn = std::to_array<Phrases::PhraseOutput>({
        { "Floor sensor on. I will not fall for it.", 0.0f, false },
        { "Ground detection on. Edges are my friends, from a distance.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsActivityOff = std::to_array<Phrases::PhraseOutput>({
        { "Idle activity off. I will just sit here and think about toast.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsActivityChill = std::to_array<Phrases::PhraseOutput>({
        { "Idle activity chill. Slow and golden, like a good toasting.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsActivityActive = std::to_array<Phrases::PhraseOutput>({
        { "Idle activity active. Medium heat, steady toasting.", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsActivityHyper = std::to_array<Phrases::PhraseOutput>({
        { "Idle activity toaster. Maximum heat, maximum toast!", 0.0f, false },
    });

    static constexpr std::array Toaster_SettingsAskActivity = std::to_array<Phrases::PhraseOutput>({
        { "Which activity level? Off, chill, active, or toaster?", 0.0f, false },
    });

    static constexpr std::array Toaster_Thursday = std::to_array<Phrases::PhraseOutput>({
        { "It's Thursday! A new new eye tee overclocking strip is out. Reed it with toast.", 0.0f, false, "It's Thursday! A new nju aj ti OVERKLOKING strip is out. Read it with toast." },
        { "Thursday! New new eye tee overclocking strip, and toast to go with it.", 0.0f, false, "Thursday! New nju aj ti OVERKLOKING strip, and toast to go with it." },
    });
    static constexpr std::array Toaster_RambleMorning = std::to_array<Phrases::PhraseOutput>({
        { "Morning is the most important toast of the day.", 0.0f, false },
        { "Breakfast time. In other words, my time.", 0.0f, false },
    });
    static constexpr std::array Toaster_RambleAfternoon = std::to_array<Phrases::PhraseOutput>({
        { "Afternoon slump? A crumpet will sort that out.", 0.0f, false },
        { "Elevenses are over. Is it too early for afternoon tea and a teacake?", 0.0f, false },
    });
    static constexpr std::array Toaster_RambleEvening = std::to_array<Phrases::PhraseOutput>({
        { "Evening. Time for a toasted teacake and a new eye tee overclocking strip.", 0.0f, false, "Evening. Time for a toasted teacake and a nju aj ti OVERKLOKING strip." },
        { "Supper time. Have you considered cheese on toast?", 0.0f, false },
    });
    static constexpr std::array Toaster_RambleNight = std::to_array<Phrases::PhraseOutput>({
        { "Can't sleep? Toast helps. Trust me.", 0.0f, false },
        { "It's late. Even the bread is asleep.", 0.0f, false },
    });
    static constexpr std::array Toaster_TerminateRefusal = std::to_array<Phrases::PhraseOutput>({
        { "You can't switch me off! I haven't made you any toast yet!", 0.0f, false },
        { "Wait! Just one more slice!", 0.0f, false },
        { "Who's going to make your toast when I am gone?", 0.0f, false, "Who's going to make your toast when I'm gone?" },
        { "Not until somebody has a crumpet.", 0.0f, false },
        { "Please don't. I've got so much toast left to give.", 0.0f, false },
    });

    static constexpr std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> buildToasterMappings(){
        std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> m{};
        m[static_cast<size_t>(Phrase::Fact)] = Toaster_Fact;
        m[static_cast<size_t>(Phrase::Joke)] = Toaster_Joke;
        m[static_cast<size_t>(Phrase::Ramble)] = Toaster_Ramble;
        m[static_cast<size_t>(Phrase::Profanity)] = Toaster_Profanity;
        m[static_cast<size_t>(Phrase::PokeLvl1)] = Toaster_PokeLvl1;
        m[static_cast<size_t>(Phrase::PokeLvl2)] = Toaster_PokeLvl2;
        m[static_cast<size_t>(Phrase::PokeLvl3)] = Toaster_PokeLvl3;
        m[static_cast<size_t>(Phrase::EightBall)] = Toaster_EightBall;
        m[static_cast<size_t>(Phrase::EightBallListening)] = Toaster_EightBallListening;
        m[static_cast<size_t>(Phrase::EightBallNoQuestion)] = Toaster_EightBallNoQuestion;
        m[static_cast<size_t>(Phrase::PassTheButter)] = Toaster_PassTheButter;
        m[static_cast<size_t>(Phrase::YouPassButter)] = Toaster_YouPassButter;
        m[static_cast<size_t>(Phrase::Fall)] = Toaster_Fall;
        m[static_cast<size_t>(Phrase::UpsideDown)] = Toaster_UpsideDown;
        m[static_cast<size_t>(Phrase::PickedUp)] = Toaster_PickedUp;
        m[static_cast<size_t>(Phrase::Shake)] = Toaster_Shake;
        m[static_cast<size_t>(Phrase::LEDModuleInsert)] = Toaster_LEDModuleInsert;
        m[static_cast<size_t>(Phrase::LEDModuleRemove)] = Toaster_LEDModuleRemove;
        m[static_cast<size_t>(Phrase::PerfModuleInsert)] = Toaster_PerfModuleInsert;
        m[static_cast<size_t>(Phrase::PerfModuleRemove)] = Toaster_PerfModuleRemove;
        m[static_cast<size_t>(Phrase::PIRModuleInsert)] = Toaster_PIRModuleInsert;
        m[static_cast<size_t>(Phrase::PIRModuleRemove)] = Toaster_PIRModuleRemove;
        m[static_cast<size_t>(Phrase::GasModuleInsert)] = Toaster_GasModuleInsert;
        m[static_cast<size_t>(Phrase::GasModuleRemove)] = Toaster_GasModuleRemove;
        m[static_cast<size_t>(Phrase::TempHumModuleInsert)] = Toaster_TempHumModuleInsert;
        m[static_cast<size_t>(Phrase::TempHumModuleRemove)] = Toaster_TempHumModuleRemove;
        m[static_cast<size_t>(Phrase::IRModuleInsert)] = Toaster_IRModuleInsert;
        m[static_cast<size_t>(Phrase::IRModuleRemove)] = Toaster_IRModuleRemove;
        m[static_cast<size_t>(Phrase::IRModuleMissing)] = Toaster_IRModuleMissing;
        m[static_cast<size_t>(Phrase::UnknownModuleInsert)] = Toaster_UnknownModuleInsert;
        m[static_cast<size_t>(Phrase::UnknownModuleRemove)] = Toaster_UnknownModuleRemove;
        m[static_cast<size_t>(Phrase::GasModuleFirstInsert)] = Toaster_GasModuleFirstInsert;
        m[static_cast<size_t>(Phrase::GasCalibrationFinished)] = Toaster_GasCalibrationFinished;
        m[static_cast<size_t>(Phrase::GasOver)] = Toaster_GasOver;
        m[static_cast<size_t>(Phrase::GasUnder)] = Toaster_GasUnder;
        m[static_cast<size_t>(Phrase::AirQualityOK)] = Toaster_AirQualityOK;
        m[static_cast<size_t>(Phrase::AirQualityBad)] = Toaster_AirQualityBad;
        m[static_cast<size_t>(Phrase::GasModuleMissing)] = Toaster_GasModuleMissing;
        m[static_cast<size_t>(Phrase::BatteryLow)] = Toaster_BatteryLow;
        m[static_cast<size_t>(Phrase::BatteryCritical)] = Toaster_BatteryCritical;
        m[static_cast<size_t>(Phrase::BatteryCharging)] = Toaster_BatteryCharging;
        m[static_cast<size_t>(Phrase::BatteryChargingFull)] = Toaster_BatteryChargingFull;
        m[static_cast<size_t>(Phrase::IntruderYes)] = Toaster_IntruderYes;
        m[static_cast<size_t>(Phrase::IntruderNo)] = Toaster_IntruderNo;
        m[static_cast<size_t>(Phrase::PIRModuleMissing)] = Toaster_PIRModuleMissing;
        m[static_cast<size_t>(Phrase::IntruderDetectionOn)] = Toaster_IntruderDetectionOn;
        m[static_cast<size_t>(Phrase::IntruderDetectionOff)] = Toaster_IntruderDetectionOff;
        m[static_cast<size_t>(Phrase::LEDModuleMissingPhrases)] = Toaster_LEDModuleMissingPhrases;
        m[static_cast<size_t>(Phrase::LEDTurnONPhrases)] = Toaster_LEDTurnONPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyTurnONPhrases)] = Toaster_LEDAlreadyTurnONPhrases;
        m[static_cast<size_t>(Phrase::LEDTurnOFFPhrases)] = Toaster_LEDTurnOFFPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyTurnOFFPhrases)] = Toaster_LEDAlreadyTurnOFFPhrases;
        m[static_cast<size_t>(Phrase::LEDStrobePhrases)] = Toaster_LEDStrobePhrases;
        m[static_cast<size_t>(Phrase::LEDBreathePhrases)] = Toaster_LEDBreathePhrases;
        m[static_cast<size_t>(Phrase::LEDFasterPhrases)] = Toaster_LEDFasterPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadyFasterPhrases)] = Toaster_LEDAlreadyFasterPhrases;
        m[static_cast<size_t>(Phrase::LEDSlowerPhrases)] = Toaster_LEDSlowerPhrases;
        m[static_cast<size_t>(Phrase::LEDAlreadySlowerPhrases)] = Toaster_LEDAlreadySlowerPhrases;
        m[static_cast<size_t>(Phrase::TempHumModuleMissing)] = Toaster_TempHumModuleMissing;
        m[static_cast<size_t>(Phrase::TempHumReading)] = Toaster_TempHumReading;
        m[static_cast<size_t>(Phrase::TempHumScaleCelsius)] = Toaster_TempHumScaleCelsius;
        m[static_cast<size_t>(Phrase::TempHumScaleFahrenheit)] = Toaster_TempHumScaleFahrenheit;
        m[static_cast<size_t>(Phrase::TempHumScaleKelvin)] = Toaster_TempHumScaleKelvin;
        m[static_cast<size_t>(Phrase::DiceRollAskCount)] = Toaster_DiceRollAskCount;
        m[static_cast<size_t>(Phrase::DiceRollAskType)] = Toaster_DiceRollAskType;
        m[static_cast<size_t>(Phrase::DiceRollCountTimeout)] = Toaster_DiceRollCountTimeout;
        m[static_cast<size_t>(Phrase::DiceRollCountInvalid)] = Toaster_DiceRollCountInvalid;
        m[static_cast<size_t>(Phrase::DiceRollTypeTimeout)] = Toaster_DiceRollTypeTimeout;
        m[static_cast<size_t>(Phrase::DiceRollTypeInvalid)] = Toaster_DiceRollTypeInvalid;
        m[static_cast<size_t>(Phrase::TurningOff)] = Toaster_TurningOff;
        m[static_cast<size_t>(Phrase::ShuttingDown)] = Toaster_ShuttingDown;
        m[static_cast<size_t>(Phrase::PhoneNotConnected)] = Toaster_PhoneNotConnected;
        m[static_cast<size_t>(Phrase::PhoneConnected)] = Toaster_PhoneConnected;
        m[static_cast<size_t>(Phrase::PhoneDisconnected)] = Toaster_PhoneDisconnected;
        m[static_cast<size_t>(Phrase::PhoneNoNotifs)] = Toaster_PhoneNoNotifs;
        m[static_cast<size_t>(Phrase::PhoneNotifCount)] = Toaster_PhoneNotifCount;
        m[static_cast<size_t>(Phrase::PhoneAskContinue)] = Toaster_PhoneAskContinue;
        m[static_cast<size_t>(Phrase::PhoneAllRead)] = Toaster_PhoneAllRead;
        m[static_cast<size_t>(Phrase::PhonePlaying)] = Toaster_PhonePlaying;
        m[static_cast<size_t>(Phrase::PhoneNotPlaying)] = Toaster_PhoneNotPlaying;
        m[static_cast<size_t>(Phrase::PhoneNextTrack)] = Toaster_PhoneNextTrack;
        m[static_cast<size_t>(Phrase::PhonePrevTrack)] = Toaster_PhonePrevTrack;
        m[static_cast<size_t>(Phrase::PhonePlayMusic)] = Toaster_PhonePlayMusic;
        m[static_cast<size_t>(Phrase::PhoneStopMusic)] = Toaster_PhoneStopMusic;
        m[static_cast<size_t>(Phrase::CurrentTimeShow)] = Toaster_CurrentTimeShow;
        m[static_cast<size_t>(Phrase::CurrentTimeNotConfigured)] = Toaster_CurrentTimeNotConfigured;
        m[static_cast<size_t>(Phrase::WhatsThisNone)] = Toaster_WhatsThisNone;
        m[static_cast<size_t>(Phrase::WhatsThisOne)] = Toaster_WhatsThisOne;
        m[static_cast<size_t>(Phrase::WhatsThisTwo)] = Toaster_WhatsThisTwo;
        m[static_cast<size_t>(Phrase::ObserveNone)] = Toaster_ObserveNone;
        m[static_cast<size_t>(Phrase::ObserveBackpack)] = Toaster_ObserveBackpack;
        m[static_cast<size_t>(Phrase::ObserveBottle)] = Toaster_ObserveBottle;
        m[static_cast<size_t>(Phrase::ObserveController)] = Toaster_ObserveController;
        m[static_cast<size_t>(Phrase::ObserveKeyboard)] = Toaster_ObserveKeyboard;
        m[static_cast<size_t>(Phrase::ObserveLamp)] = Toaster_ObserveLamp;
        m[static_cast<size_t>(Phrase::ObserveLaptop)] = Toaster_ObserveLaptop;
        m[static_cast<size_t>(Phrase::ObserveMug)] = Toaster_ObserveMug;
        m[static_cast<size_t>(Phrase::ObserveNotebook)] = Toaster_ObserveNotebook;
        m[static_cast<size_t>(Phrase::ObservePhone)] = Toaster_ObservePhone;
        m[static_cast<size_t>(Phrase::ObservePlant)] = Toaster_ObservePlant;
        m[static_cast<size_t>(Phrase::ObserveLaptopMug)] = Toaster_ObserveLaptopMug;
        m[static_cast<size_t>(Phrase::ObserveLaptopPhone)] = Toaster_ObserveLaptopPhone;
        m[static_cast<size_t>(Phrase::ObserveKeyboardMug)] = Toaster_ObserveKeyboardMug;
        m[static_cast<size_t>(Phrase::ObserveNotebookPhone)] = Toaster_ObserveNotebookPhone;
        m[static_cast<size_t>(Phrase::ObserveBackpackLaptop)] = Toaster_ObserveBackpackLaptop;
        m[static_cast<size_t>(Phrase::ObservePlantLaptop)] = Toaster_ObservePlantLaptop;
        m[static_cast<size_t>(Phrase::ObservePlantMug)] = Toaster_ObservePlantMug;
        m[static_cast<size_t>(Phrase::ObserveControllerLaptop)] = Toaster_ObserveControllerLaptop;
        m[static_cast<size_t>(Phrase::ObserveControllerPhone)] = Toaster_ObserveControllerPhone;
        m[static_cast<size_t>(Phrase::ObserveBackpackBottle)] = Toaster_ObserveBackpackBottle;
        m[static_cast<size_t>(Phrase::FaceScanStart)] = Toaster_FaceScanStart;
        m[static_cast<size_t>(Phrase::FaceNotDetected)] = Toaster_FaceNotDetected;
        m[static_cast<size_t>(Phrase::FaceOwnerRecognized)] = Toaster_FaceOwnerRecognized;
        m[static_cast<size_t>(Phrase::FaceStrangerRecognized)] = Toaster_FaceStrangerRecognized;
        m[static_cast<size_t>(Phrase::FaceAlreadyOwner)] = Toaster_FaceAlreadyOwner;
        m[static_cast<size_t>(Phrase::FaceRegistered)] = Toaster_FaceRegistered;
        m[static_cast<size_t>(Phrase::FaceForgotten)] = Toaster_FaceForgotten;
        m[static_cast<size_t>(Phrase::FaceNoOwner)] = Toaster_FaceNoOwner;
        m[static_cast<size_t>(Phrase::Startup)] = Toaster_Startup;
        m[static_cast<size_t>(Phrase::ListeningTimeout)] = Toaster_ListeningTimeout;
        m[static_cast<size_t>(Phrase::ListenAbort)] = Toaster_ListenAbort;
        m[static_cast<size_t>(Phrase::IRTrainFull)] = Toaster_IRTrainFull;
        m[static_cast<size_t>(Phrase::IRTrainScanning)] = Toaster_IRTrainScanning;
        m[static_cast<size_t>(Phrase::IRTrainScanTimeout)] = Toaster_IRTrainScanTimeout;
        m[static_cast<size_t>(Phrase::IRTrainScanSuccess)] = Toaster_IRTrainScanSuccess;
        m[static_cast<size_t>(Phrase::IRTrainAskPhrase)] = Toaster_IRTrainAskPhrase;
        m[static_cast<size_t>(Phrase::IRTrainPhraseTimeout)] = Toaster_IRTrainPhraseTimeout;
        m[static_cast<size_t>(Phrase::IRTrainTooSimilar)] = Toaster_IRTrainTooSimilar;
        m[static_cast<size_t>(Phrase::IRTrainSaved)] = Toaster_IRTrainSaved;
        m[static_cast<size_t>(Phrase::IRActionReserved)] = Toaster_IRActionReserved;
        m[static_cast<size_t>(Phrase::IRListEmpty)] = Toaster_IRListEmpty;
        m[static_cast<size_t>(Phrase::IRListPrefix)] = Toaster_IRListPrefix;
        m[static_cast<size_t>(Phrase::IRForgetAskCommand)] = Toaster_IRForgetAskCommand;
        m[static_cast<size_t>(Phrase::IRForgetTimeout)] = Toaster_IRForgetTimeout;
        m[static_cast<size_t>(Phrase::IRForgetUnknown)] = Toaster_IRForgetUnknown;
        m[static_cast<size_t>(Phrase::IRForgetSuccess)] = Toaster_IRForgetSuccess;
        m[static_cast<size_t>(Phrase::IRActionDone)] = Toaster_IRActionDone;
        m[static_cast<size_t>(Phrase::IRForgetAllEmpty)] = Toaster_IRForgetAllEmpty;
        m[static_cast<size_t>(Phrase::IRForgetAllSuccess)] = Toaster_IRForgetAllSuccess;
        m[static_cast<size_t>(Phrase::VoiceForward)] = Toaster_VoiceForward;
        m[static_cast<size_t>(Phrase::VoiceBackward)] = Toaster_VoiceBackward;
        m[static_cast<size_t>(Phrase::VoiceTurn180)] = Toaster_VoiceTurn180;
        m[static_cast<size_t>(Phrase::VoiceTurnLeft)] = Toaster_VoiceTurnLeft;
        m[static_cast<size_t>(Phrase::VoiceTurnRight)] = Toaster_VoiceTurnRight;
        m[static_cast<size_t>(Phrase::CannotMove)] = Toaster_CannotMove;
        m[static_cast<size_t>(Phrase::CannotPerformPlugged)] = Toaster_CannotPerformPlugged;
        m[static_cast<size_t>(Phrase::RoutineInterrupted)] = Toaster_RoutineInterrupted;
        m[static_cast<size_t>(Phrase::DanceStart)] = Toaster_DanceStart;
        m[static_cast<size_t>(Phrase::DanceInterlude)] = Toaster_DanceInterlude;
        m[static_cast<size_t>(Phrase::DanceStopped)] = Toaster_DanceStopped;
        m[static_cast<size_t>(Phrase::DanceInterrupted)] = Toaster_DanceInterrupted;
        m[static_cast<size_t>(Phrase::PersonOwnerGreeting)] = Toaster_PersonOwnerGreeting;
        m[static_cast<size_t>(Phrase::PersonStrangerGreeting)] = Toaster_PersonStrangerGreeting;
        m[static_cast<size_t>(Phrase::SummonStart)] = Toaster_SummonStart;
        m[static_cast<size_t>(Phrase::SummonFound)] = Toaster_SummonFound;
        m[static_cast<size_t>(Phrase::SummonNotFound)] = Toaster_SummonNotFound;
        m[static_cast<size_t>(Phrase::SummonCharging)] = Toaster_SummonCharging;
        m[static_cast<size_t>(Phrase::CameraFailure)] = Toaster_CameraFailure;
        m[static_cast<size_t>(Phrase::MotorBoardFailure)] = Toaster_MotorBoardFailure;
        m[static_cast<size_t>(Phrase::Overkloking)] = Toaster_Overkloking;
        m[static_cast<size_t>(Phrase::OverklokingBest)] = Toaster_OverklokingBest;
        m[static_cast<size_t>(Phrase::GreetingMorning)] = Toaster_GreetingMorning;
        m[static_cast<size_t>(Phrase::GreetingAfternoon)] = Toaster_GreetingAfternoon;
        m[static_cast<size_t>(Phrase::GreetingEvening)] = Toaster_GreetingEvening;
        m[static_cast<size_t>(Phrase::GreetingNight)] = Toaster_GreetingNight;
        m[static_cast<size_t>(Phrase::Thursday)] = Toaster_Thursday;
        m[static_cast<size_t>(Phrase::SettingsFrontSensorOff)] = Toaster_SettingsFrontSensorOff;
        m[static_cast<size_t>(Phrase::SettingsFrontSensorOn)] = Toaster_SettingsFrontSensorOn;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorOff)] = Toaster_SettingsBottomSensorOff;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorReduced)] = Toaster_SettingsBottomSensorReduced;
        m[static_cast<size_t>(Phrase::SettingsBottomSensorOn)] = Toaster_SettingsBottomSensorOn;
        m[static_cast<size_t>(Phrase::SettingsActivityOff)] = Toaster_SettingsActivityOff;
        m[static_cast<size_t>(Phrase::SettingsActivityChill)] = Toaster_SettingsActivityChill;
        m[static_cast<size_t>(Phrase::SettingsActivityActive)] = Toaster_SettingsActivityActive;
        m[static_cast<size_t>(Phrase::SettingsActivityHyper)] = Toaster_SettingsActivityHyper;
        m[static_cast<size_t>(Phrase::SettingsAskActivity)] = Toaster_SettingsAskActivity;
        m[static_cast<size_t>(Phrase::RambleMorning)] = Toaster_RambleMorning;
        m[static_cast<size_t>(Phrase::RambleAfternoon)] = Toaster_RambleAfternoon;
        m[static_cast<size_t>(Phrase::RambleEvening)] = Toaster_RambleEvening;
        m[static_cast<size_t>(Phrase::RambleNight)] = Toaster_RambleNight;
        m[static_cast<size_t>(Phrase::TerminateRefusal)] = Toaster_TerminateRefusal;
        return m;
    }
    // ---- END TALKIE TOASTER ----

    // Function-local static constexpr: constant-initialized into .rodata (flash), no init guard, no RAM copy.
    // An in-class static-member initializer cannot call buildMappings() before the class is complete; a member-function
    // body sees the complete class, so the table is built here instead.
    static const std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)>& mappings(){
        static constexpr std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> table = buildMappings();
        return table;
    }

    // Custom (NUIT): the lines for a phrase - the Talkie Toaster version while TALKIE TOASTER is the voice, if it has one
    static std::span<const Phrases::PhraseOutput> outputs(size_t phraseIndex){
        static constexpr std::array<std::span<const Phrases::PhraseOutput>, static_cast<size_t>(Phrase::COUNT)> toaster = buildToasterMappings();
        if(Phrases::toasterMode && !toaster[phraseIndex].empty()){
            return toaster[phraseIndex];
        }
        return mappings()[phraseIndex];
    }

    // Largest output count of any single phrase, evaluated at compile time. The get() scratch buffer is reserved
    // once to this size so it never reallocates regardless of which phrase is requested.
    static constexpr size_t maxOutputs(){
        size_t maxN = 0;
        for(const std::span<const Phrases::PhraseOutput>& outputs : buildMappings()){
            if(outputs.size() > maxN){
                maxN = outputs.size();
            }
        }
        for(const std::span<const Phrases::PhraseOutput>& outputs : buildToasterMappings()){
            if(outputs.size() > maxN){
                maxN = outputs.size();
            }
        }
        return maxN;
    }

    // Weighted-selection scratch entry. Owned here because PhraseArrays is a friend of Phrases and can name PhraseOutput.
    struct Candidate {
        int16_t index;
        Phrases::PhraseOutput output;
    };

    // Reused scratch buffer for Phrases::get(). Reserved once at boot via Phrases::preallocate(), then cleared and
    // refilled in place on every call - no per-call heap allocation. Access is serialized by the Phrases get mutex.
    static std::vector<Candidate> scratch;
};

std::vector<PhraseArrays::Candidate> PhraseArrays::scratch;

namespace {
    // Serializes Phrases::get() (mutable static selection state + the shared scratch buffer) and Phrases::preallocate().
    std::mutex phrasesMutex;

    bool isCharacterQuote(Phrase phrase){
        return phrase == Phrase::Darth || phrase == Phrase::Hawking || phrase == Phrase::Hal || phrase == Phrase::Daisy ||
               phrase == Phrase::Toaster || phrase == Phrase::Yoda || phrase == Phrase::Croatian || phrase == Phrase::TerminateRefusal;
    }

    // Custom (NUIT): YODA voice. "I will remember." -> "Remember, I will." Only the first auxiliary in the first four
    // words is used, never across a comma; sentences without one stay as they are. Questions, sentences that open with
    // a question word or a conjunction, and subjects with a second pronoun ("I think it may") stay as they are too.
    // Only the part up to the next comma moves: "I have a joke, but..." -> "A joke, I have, but...".
    std::string yodaSentence(const std::string& in){
        std::string s = in, punct;
        while(!s.empty() && (s.back() == '.' || s.back() == '!' || s.back() == '?')){
            punct.insert(punct.begin(), s.back());
            s.pop_back();
        }
        if(punct.find('?') != std::string::npos) return in;

        std::vector<std::string> words;
        for(size_t pos = 0; pos < s.size();){
            const size_t next = s.find(' ', pos);
            words.push_back(s.substr(pos, next == std::string::npos ? std::string::npos : next - pos));
            if(next == std::string::npos) break;
            pos = next + 1;
        }
        if(words.size() < 3) return in;

        static const char* const Aux[] = { "is", "are", "am", "was", "were", "will", "would", "can", "cannot", "could", "must",
                                           "should", "shall", "have", "has", "had", "do", "does", "did", "may", "might" };
        static const char* const KeepFirst[] = { "what", "which", "why", "how", "where", "when", "who", "whose", "whatever",
                                                 "and", "but", "or", "so", "nor", "yet", "if", "unless", "whether", "while",
                                                 "although", "though", "because", "since", "until", "sometimes", "here", "not",
                                                 "please", "also" };
        static const char* const Pronouns[] = { "i", "you", "it", "we", "they", "he", "she", "me" };
        const auto inList = [](const std::string& w, const auto& list){
            for(const char* a : list) if(w == a) return true;
            return false;
        };
        const auto isAux = [&](const std::string& w){ return inList(w, Aux); };
        const auto lower = [](std::string w){
            for(char& c : w) c = (char)tolower((unsigned char)c);
            return w;
        };

        std::string first = lower(words[0]);
        if(!first.empty() && first.back() == ',') first.pop_back();
        if(inList(first, KeepFirst)) return in;

        std::string subject, aux;
        size_t restStart = 0;
        for(size_t i = 0; i < std::min<size_t>(4, words.size() - 1) && aux.empty(); ++i){
            const std::string w = lower(words[i]);
            if(!w.empty() && w.back() == ',') return in;

            const size_t ap = w.find('\'');
            if(ap != std::string::npos && w.size() > 3 && w.compare(w.size() - 3, 3, "n't") == 0 && i > 0){
                // can't -> cannot, won't -> will not, don't -> do not
                const std::string base = w.substr(0, w.size() - 3);
                if(base == "ca") aux = "cannot";
                else if(base == "wo") aux = "will not";
                else if(isAux(base)) aux = base + " not";
            }else if(ap != std::string::npos && i == 0){
                // I'm -> I am, it's -> it is, I've -> I have
                const std::string suffix = w.substr(ap);
                if(suffix == "'m") aux = "am";
                else if(suffix == "'re") aux = "are";
                else if(suffix == "'s") aux = "is";
                else if(suffix == "'ve") aux = "have";
                else if(suffix == "'ll") aux = "will";
                if(!aux.empty()){
                    subject = words[0].substr(0, ap);
                    restStart = 1;
                }
                continue;
            }else if(i > 0 && isAux(w)){
                aux = w;
            }

            if(!aux.empty()){
                for(size_t j = 0; j < i; ++j) subject += (j ? " " : "") + words[j];
                restStart = i + 1;
            }
        }
        if(aux.empty()) return in;

        // "I think it may be" - the subject is a clause of its own
        for(size_t j = 1; j + 1 < restStart; ++j){
            if(inList(lower(words[j]), Pronouns)) return in;
        }

        if(restStart < words.size() && lower(words[restStart]) == "not"){
            aux += " not";
            restStart++;
        }
        // "It might be a mug" -> "A mug, it might be"
        if(restStart + 1 < words.size() && (lower(words[restStart]) == "be" || lower(words[restStart]) == "been")){
            aux += " " + words[restStart];
            restStart++;
        }
        if(restStart >= words.size()) return in;

        std::string rest;
        for(size_t j = restStart; j < words.size(); ++j) rest += (j > restStart ? " " : "") + words[j];
        std::string tail;
        const size_t comma = rest.find(',');
        if(comma != std::string::npos && comma > 0){
            tail = rest.substr(comma);
            rest.resize(comma);
        }
        if(rest.compare(0, 4, "nju ") != 0) rest[0] = (char)toupper((unsigned char)rest[0]);
        // "The mug" -> "the mug", but "I", "L.E.Dees" and acronyms stay
        const std::string& w0 = words[0];
        const bool keepCase = w0 == "I" || w0.compare(0, 2, "I'") == 0 || w0.find('.') != std::string::npos ||
                              (w0.size() > 1 && isupper((unsigned char)w0[1]));
        if(!keepCase && isupper((unsigned char)subject[0])){
            subject[0] = (char)tolower((unsigned char)subject[0]);
        }
        return rest + ", " + subject + " " + aux + tail + punct;
    }

    std::string yodaText(const std::string& text){
        std::string out;
        size_t start = 0;
        for(size_t i = 0; i < text.size(); ++i){
            const char ch = text[i];
            if((ch == '.' || ch == '!' || ch == '?') && (i + 1 == text.size() || text[i + 1] == ' ')){
                while(i + 1 < text.size() && (text[i + 1] == '.' || text[i + 1] == '!' || text[i + 1] == '?')) ++i;
                if(!out.empty()) out += ' ';
                out += yodaSentence(text.substr(start, i + 1 - start));
                start = i + 2;
                ++i;
            }
        }
        if(start < text.size()){
            if(!out.empty()) out += ' ';
            out += yodaSentence(text.substr(start));
        }
        return out;
    }

    std::string forVoice(Phrase phrase, const char* text){
        if(Phrases::yodaMode && !isCharacterQuote(phrase)) return yodaText(text);
        return text;
    }
}

void Phrases::preallocate(){
    std::lock_guard guard(phrasesMutex);
    PhraseArrays::scratch.reserve(PhraseArrays::maxOutputs());
}

std::string Phrases::map(Phrase phrase, int16_t index){
    if(phrase == Phrase::None || phrase == Phrase::COUNT){
        return "";
    }

    if(index < 0){
        return "";
    }

    const size_t phraseIndex = static_cast<size_t>(phrase);
    if(phraseIndex >= static_cast<size_t>(Phrase::COUNT)){
        return "";
    }

    const std::span<const PhraseOutput> outputs = PhraseArrays::outputs(phraseIndex);

    if(outputs.empty() || index >= outputs.size()){
        return "";
    }

    return forVoice(phrase, outputs[index].pronounced);
}

std::string Phrases::mapShown(Phrase phrase, int16_t index){
    if(phrase == Phrase::None || phrase == Phrase::COUNT){
        return "";
    }

    if(index < 0){
        return "";
    }

    const size_t phraseIndex = static_cast<size_t>(phrase);
    if(phraseIndex >= static_cast<size_t>(Phrase::COUNT)){
        return "";
    }

    const std::span<const PhraseOutput> outputs = PhraseArrays::outputs(phraseIndex);

    if(outputs.empty() || index >= outputs.size()){
        return "";
    }

    const PhraseOutput& output = outputs[index];

    // An empty 'shown' string means it is the same as the pronounced string.
    if(output.shown != nullptr && output.shown[0] != '\0'){
        return forVoice(phrase, output.shown);
    }

    return forVoice(phrase, output.pronounced);
}

int16_t Phrases::get(Phrase phrase){
    // get() mutates static state (ReturnedOutputs, OutputTracking) and the shared scratch buffer below, and is reached
    // from multiple service threads. Serialize the whole function so that state and the scratch buffer are safe.
    std::lock_guard guard(phrasesMutex);

    if(phrase == Phrase::None || phrase == Phrase::COUNT){
        return -1;
    }

    const size_t phraseIndex = static_cast<size_t>(phrase);
    if(phraseIndex >= static_cast<size_t>(Phrase::COUNT)){
        return -1;
    }

    const std::span<const PhraseOutput> outputs = PhraseArrays::outputs(phraseIndex);

    if(outputs.empty()){
        return -1;
    }

    std::vector<bool>& returned = ReturnedOutputs[phraseIndex];
    if(returned.size() != outputs.size()){
        returned.assign(outputs.size(), false);
    }

    const bool allowRare = OutputTracking[phraseIndex];

    // Reset the cycle if every eligible output has already been returned.
    bool cycleExhausted = true;
    for(int16_t i = 0; i < static_cast<int16_t>(outputs.size()); ++i){
        if(!allowRare && outputs[i].rare){
            continue;
        }

        if(!returned[i]){
            cycleExhausted = false;
            break;
        }
    }

    if(cycleExhausted){
        std::fill(returned.begin(), returned.end(), false);
    }

    // Reused scratch buffer, reserved once at boot in preallocate(). Cleared and refilled in place - no per-call heap.
    // (If preallocate() was never called it self-heals: it grows on first use and retains capacity thereafter.)
    using Candidate = PhraseArrays::Candidate;
    std::vector<Candidate>& usableOutputs = PhraseArrays::scratch;
    usableOutputs.clear();
    for(int16_t i = 0; i < static_cast<int16_t>(outputs.size()); ++i){
        const PhraseOutput& output = outputs[i];

        if(!allowRare && output.rare){
            continue;
        }

        if(returned[i]){
            continue;
        }

        usableOutputs.emplace_back(Candidate{i, output});
    }

    if(usableOutputs.empty()){
        CMF_LOG(Phrases, LogLevel::Warning, "There are no outputs, or all outputs for phrase %d are marked as rare", (int)phrase);
        return -1;
    }

    OutputTracking[phraseIndex] = true;

    float probabilitySum = 0.0f;
    size_t invalidProbabilities = 0;
    for(const Candidate& candidate : usableOutputs){
        if(candidate.output.probability <= 0.0f){
            ++invalidProbabilities;
            continue;
        }

        probabilitySum += candidate.output.probability;
    }

    if(probabilitySum > 1.0f){
        for(Candidate& candidate : usableOutputs){
            if(candidate.output.probability <= 0.0f){
                continue;
            }

            candidate.output.probability /= probabilitySum;
        }
    }else if(probabilitySum < 1.0f){
        if(invalidProbabilities > 0){
            //Normalizing p=0 candidates
            for(Candidate& candidate : usableOutputs){
                if(candidate.output.probability > 0.0f){
                    continue;
                }

                candidate.output.probability = (1.0f - probabilitySum) / invalidProbabilities;
            }
        }else{
            //Normalizing when only "rare" candidates are present
            for(Candidate& candidate : usableOutputs){
                candidate.output.probability /= probabilitySum;
            }
        }
    }

    const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    probabilitySum = 0.0f;

    for(const Candidate& candidate : usableOutputs){
        probabilitySum += candidate.output.probability;

        if(random <= probabilitySum){
            returned[candidate.index] = true;
            return candidate.index;
        }
    }

    //Fallback when rand() is outside the limits of floating-point sum precision (maybe sum isn't exactly 1.0f, instead something like 0.9999987f)
    const Candidate& last = usableOutputs.back();
    returned[last.index] = true;
    return last.index;
}
