import toml
import os
from pathlib import Path
from appdirs import user_config_dir

INSTRUMENT_CONFIGURATION_FOLDER = "instrument-configurations"

class ConfigurationParser:
	def get_instrument_configurations():
		instruments = []

		print(__name__)
		assert False
		#configuration_folder = Path(user_config_dir(__name__))
		assert configuration_folder.is_dir()
		print(os.fspath(configuration_folder))

		for instrument_config in os.listdir(configuration_folder):
			print(instrument_config)
			#if not instrument_config.is_dir():
			with open(instrument_config, 'r') as f:
				print(f.read())
				parsed_config = toml.loads(f.read())
				validate_configuration(parsed_config)
				instruments.append(parsed_config)

		return instruments

	def validate_configuration(config_toml):
		is_valid = true
		is_valid &= config_toml["instrument-information"]
		is_valid &= config_toml["instrument-information"].friendly-name
		is_valid &= config_toml["instrument-information"].visa-name

		if not valid:
			raise ValueError("Invalid configuration file.")
