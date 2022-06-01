c++ snippets for various purposes

# Compile (windows)
```
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```
Alternatively, cmake call can use paths
```
cmake -B [build_dir] -S [src_dir]
```

## Clear build directory
```
rmdir /s /q ".\build\"
```

## Run 
```
.\Debug\executable.exe
```

# Compile (linux)
```
mkdir build
cd build
cmake ..
make
```

