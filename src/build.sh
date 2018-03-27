clang main.c -framework SDL2 -o c8
if [[ $? -eq 0 ]]
then
    ./c8
fi
