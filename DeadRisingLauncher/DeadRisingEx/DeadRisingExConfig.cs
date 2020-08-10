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
        private const string ModLoadOrderSection = "ModLoadOrder";

        private const string EnableConsoleKey = "EnableConsole";

        /// <summary>
        /// Full file path to the game's config file path
        /// </summary>
        public string ConfigFilePath { get; private set; }

        /// <summary>
        /// Determines if the console window will be displayed or not
        /// </summary>
        public bool EnableConsoleWindow { get; set; }
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
            this.EnableConsoleWindow = GetConfigBool(configData, GameSettingsSection, EnableConsoleKey);

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
            configData[GameSettingsSection].AddKey(EnableConsoleKey, this.EnableConsoleWindow.ToString());

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
                fileParser.WriteFile(this.ConfigFilePath, configData);
            }
            catch (Exception e)
            {
                // Failed to write the config file.
                return false;
            }

            // Config file successfully written.
            return true;
        }

        private bool GetConfigBool(IniData config, string section, string value)
        {
            // Try to get the string representation of the config value.
            if (config.Sections.ContainsSection(section) == false || config[section].ContainsKey(value) == false)
                return false;

            // Return the parsed representation of the value.
            return bool.Parse(config[section][value]);
        }
    }
}
