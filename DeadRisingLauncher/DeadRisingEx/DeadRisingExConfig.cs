using IniParser;
using IniParser.Model;
using IniParser.Model.Configuration;
using IniParser.Parser;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeadRisingLauncher
{
    public class DeadRisingExConfig
    {
        private const string GameSettingsSection = "GameSettings";
        private const string GraphicsSettingsSection = "GraphicsSettings";
        private const string ModLoadOrderSection = "ModLoadOrder";
        
        private const string DebugLogKey = "DebugLog";
        private const string ConsoleHistoryLimitKey = "ConsoleHistoryLimit";
        private const string RecursiveGrenadeKey = "RecursiveGrenade";
        private const string ItemRandomizerKey = "ItemRandomizer";

        // Graphics settings:
        private const string DynamicGraphicsMemoryKey = "DynamicGraphicsMemory";

        /// <summary>
        /// Full file path to the game's config file path
        /// </summary>
        public string ConfigFilePath { get; private set; }

        /// <summary>
        /// Determines if debug logging is enabled or not.
        /// </summary>
        public bool DebugLog { get; set; } = false;
        /// <summary>
        /// Maximum number of items to display in the console history log.
        /// </summary>
        public int ConsoleHistoryLimit { get; set; } = 256;
        /// <summary>
        /// Determines if the recursive grenade mod will be enabled or not
        /// </summary>
        public bool RecursiveGrenade { get; set; } = false;
        /// <summary>
        /// Determines if item drops are randomized or not
        /// </summary>
        public bool ItemRandomizer { get; set; } = false;

        /// <summary>
        /// Determines if graphics memory should be allocated/resized dynamically as needed
        /// </summary>
        public bool DynamicGraphicsMemory { get; set; } = true;

        /// <summary>
        /// Mod file load order
        /// </summary>
        public string[] ModFileLoadOrder { get; set; } = new string[0];

        public DeadRisingExConfig(string configFile)
        {
            // Initialize fields.
            this.ConfigFilePath = configFile;
        }

        public bool ReadFromFile()
        {
            IniData configData = null;

            // Setup the config file parser.
            IniParserConfiguration parserConfig = new IniParserConfiguration();
            parserConfig.SkipInvalidLines = false;
            parserConfig.ThrowExceptionsOnError = true;

            IniDataParser parser = new IniDataParser(parserConfig);
            FileIniDataParser fileParser = new FileIniDataParser(parser);

            // Check if the file exists, if not return true as if we parsed it.
            if (File.Exists(this.ConfigFilePath) == false)
                return true;

            try
            {
                // Try to parse the file.
                configData = fileParser.ReadFile(this.ConfigFilePath);
            }
            catch (Exception e)
            {
                // Failed to parse the file, it must be invalid in some way.
                return false;
            }

            // Read all the values from the ini file.
            this.DebugLog = GetConfigBool(configData, GameSettingsSection, DebugLogKey);
            this.ConsoleHistoryLimit = GetConfigInt(configData, GameSettingsSection, ConsoleHistoryLimitKey, 256);
            this.RecursiveGrenade = GetConfigBool(configData, GameSettingsSection, RecursiveGrenadeKey);
            this.ItemRandomizer = GetConfigBool(configData, GameSettingsSection, ItemRandomizerKey);

            this.DynamicGraphicsMemory = GetConfigBool(configData, GraphicsSettingsSection, DynamicGraphicsMemoryKey);

            // Parse the mod load order.
            if (configData.Sections.ContainsSection(ModLoadOrderSection) == true)
            {
                // Get the load order based on the order of the keys.
                this.ModFileLoadOrder = configData[ModLoadOrderSection].Select(k => k.Value).ToArray();
            }

            // Successfully parsed the ini file.
            return true;
        }

        public bool WriteToFile()
        {
            // Build the ini data from our local state.
            IniData configData = new IniData();

            // Game Settings:
            configData.Sections.AddSection(GameSettingsSection);
            configData[GameSettingsSection].AddKey(DebugLogKey, this.DebugLog.ToString());
            configData[GameSettingsSection].AddKey(ConsoleHistoryLimitKey, this.ConsoleHistoryLimit.ToString());
            configData[GameSettingsSection].AddKey(RecursiveGrenadeKey, this.RecursiveGrenade.ToString());
            configData[GameSettingsSection].AddKey(ItemRandomizerKey, this.ItemRandomizer.ToString());

            // Graphics Settings:
            configData[GraphicsSettingsSection].AddKey(DynamicGraphicsMemoryKey, this.DynamicGraphicsMemory.ToString());

            // Mod Load Order:
            configData.Sections.AddSection(ModLoadOrderSection);
            for (int i = 0; i < this.ModFileLoadOrder.Length; i++)
            {
                // Add the mod file to the list.
                configData[ModLoadOrderSection].AddKey("Mod" + i.ToString(), this.ModFileLoadOrder[i]);
            }

            try
            {
                // Write the config data to file.
                FileIniDataParser fileParser = new FileIniDataParser();
                fileParser.WriteFile(this.ConfigFilePath, configData, Encoding.ASCII);
            }
            catch (Exception e)
            {
                // Failed to write the config file.
                return false;
            }

            // Config file successfully written.
            return true;
        }

        private bool GetConfigBool(IniData config, string section, string value, bool @default = false)
        {
            // Try to get the string representation of the config value.
            if (config.Sections.ContainsSection(section) == false || config[section].ContainsKey(value) == false)
                return @default;

            // Return the parsed representation of the value.
            return bool.Parse(config[section][value]);
        }

        private int GetConfigInt(IniData config, string section, string value, int @default)
        {
            // Try to get the string representation of the config value.
            if (config.Sections.ContainsSection(section) == false || config[section].ContainsKey(value) == false)
                return @default;

            // Return the parsed representation of the value.
            if (int.TryParse(config[section][value], out int parsedValue) == true)
                return parsedValue;
            else
                return @default;
        }
    }
}
