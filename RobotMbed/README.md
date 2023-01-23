# Mbed Resources

Resources for mbed devices are now fetched from `OpenRoberta/microbit`'s release. An overview of the workflow can be [found here](https://github.com/OpenRoberta/openroberta-lab/wiki/Mbed-DAL:-Generation-and-automation#workflow).

## Adding new libraries

For adding new libraries, place the relevant library files (.h, .cpp) in the format shown below:
```bash
ext-libs
└── <lib-name>
   ├── <header-files>
   └── <implementation-files>
```

Be sure to add the necessary includes in the `compile.bat` and `compile.sh` scripts to use the new library!
