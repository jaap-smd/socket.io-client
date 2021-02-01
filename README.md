# ladybug
ladybug worker

Install libraries:
    goto subfolder: lib/socket.io-client-cpp/
    Run cmake ./
    Run (sudo) make install (if makefile generated) or open generated project (if project file generated) to build.
    Outputs is under ./build, link with the all static libs under ./build/lib and include headers under ./build/include in your client code where you want to use it.

Install program
    create subfolder: build
    goto ./build
    run cmake ..
    run make
    
    
