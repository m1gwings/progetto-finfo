files=$(ls ./src)
src_files=""
for file in $files
do
    if [ ${file: -2} = ".c" ]
    then
        src_files="$src_files ./src/$file"
    fi
done
gcc -o risolvi-espressioni $src_files -ansi -pedantic
