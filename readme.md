# Reference files
## Mac OS
For Mac OS, you can follow this tutorial: ```https://selamjie.medium.com/set-opengl-on-macos-with-vs-code-1a92850c8c6f```

Here is the `.vscode/tasks.json` file contents
```
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "C/C++: clang build active file",
			"command": "/usr/bin/clang++",
            "args": [
                "-std=c++17",
                "-fdiagnostics-color=always",
                "-Wall",
                "-g",
                "-I${workspaceFolder}/include",
                "-I/opt/homebrew/opt/glfw/include",
                "-L/opt/homebrew/opt/glfw/lib",
                "${workspaceFolder}/src/*.cpp",
                "-lglfw",
                "-o",
                "${workspaceFolder}/main.exe",
                "-framework",
                "OpenGL"
            ],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"detail": "compiler: /usr/bin/clang++"
		}
	]
}
```
The makefile is located inside makefiles/MakefileMACOS, you will need to move the file to the root and use the `make` command in the terminal to build the `.exe`