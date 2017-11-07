import toml
from os import listdir
from pathlib import Path
from appdirs import user_config_dir

class ConfigurationParser:
	def get_instrument_configurations():
		instruments = []

		# configuration folder is located in <user config file path>/instrument_hub/
		configuration_folder = Path(user_config_dir(__package__))
		if not configuration_folder.exists():
			configuration_folder.mkdir()

		for config_file_name in listdir(configuration_folder):
			config_file = Path(configuration_folder, config_file_name)
			if not config_file.is_dir():
				with open(config_file, 'r') as f:
					parsed_config = toml.loads(f.read())
					ConfigurationParser.validate_configuration(parsed_config)
					instruments.append(parsed_config)

		if len(instruments) == 1:
			print("[i] Parsed", len(instruments), "instrument configuration file.")
		else:
			print("[i] Parsed", len(instruments), "instrument configuration files.")

		return instruments

	def validate_configuration(config_toml):
		# if any of these keys are not in the config file, KeyError is fired
		try:
			config_toml["instrument-information"]
			config_toml["instrument-information"]["visa-make"]
			config_toml["instrument-information"]["visa-model"]
			config_toml["instrument-information"]["port"]
		except KeyError:
			raise ValueError("Invalid configuration file.")
