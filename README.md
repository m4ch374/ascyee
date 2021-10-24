# ascyee
Convert image to ascii representation

![sample](/Sample_Screenshots/example.png)

## Getting Started
Since the program is written in C, we need to use C compilers to compile the code to an executable. <br />
If you don't have a compiler installed, I recommend installing `gcc`.

Installation guide for Windows: https://www.guru99.com/c-gcc-install.html

### Compiling ascyee.c
Use the following command to compile `ascyee.c`: <br />
```
> gcc ascyee.c -o ascyee
```

## How to use
`ascyee` takes in 2 arguments: <br />
```
> ascyee [file_name] [downscale_ratio]
```

* `file_name`: the file name of the image you want to convert (absolute path / relative path)
* `dwonscale_ratio`: Ratio for downscaling the image (only takes in positive integer)

## Important Notes
1. Program developed and tested in Windows, compatibility with Linux is **UNCERTAIN**
2. Using Command Prompt or Windows Powershell is heavily adviced, for some odd reason Windows Terminal prints differently <br />
3. ^^ please enlighten me if you know the reason ^^
4. Files in `/stb_lib` was **NOT** written by me

- - - -
### Credits
Library used : [Stb](https://github.com/nothings/stb) coded by [Nothings](https://github.com/nothings) and other contributors of the repository
