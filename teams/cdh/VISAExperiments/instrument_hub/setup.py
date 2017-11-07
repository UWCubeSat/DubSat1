from setuptools import setup, find_packages

setup(
	name='instrument_hub',
	version='0.1.0',
	description='Relay instrumentation between lab instruments and the COSMOS suite',
	url='https://github.com/UWCubeSat/DubSat1',

	author='Mohammad Kayali',
	email='kayali@uw.edu',

	classifiers=[
		'Development Status :: 3 - Alpha',
		'Programming Language :: Python :: 3'
	],

	packages=find_packages(exclude=['docs', 'tests']),

	install_requires=['pyvisa', 'toml', 'appdirs', 'tzlocal'],

	entry_points={
        'console_scripts': [
            'instrument_hub=instrument_hub.instrumentation_hub:main',
        ],
    },
)