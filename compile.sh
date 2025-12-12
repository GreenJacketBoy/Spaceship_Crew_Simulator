paths=(
    "src/View/crew_view.c"
    "src/View/super_view.c"
    "src/View/room_view.c"

    "src/Model/crew_model.c"
    "src/Model/room_model.c"
    "src/Model/crew_room_linker_model.c"

    "src/Controller/crew_controller.c"
    "src/Controller/super_controller.c"
    "src/Controller/room_controller.c"

    "src/main.c"
    "src/core.c"
)

for path in "${paths[@]}"
do
    gcc -c $path
done

gcc *.o -o main.out
rm *.o