paths=(
    "src/View/crew_view.c"
    "src/Model/crew_model.c"
    "src/Controller/crew_controller.c"
)

for path in "${paths[@]}"
do
    gcc -c $path
done

gcc *.o -o main.out
rm *.o