#include <utils/hello.h>
#include <commons/string.h>

void hello_world() {
    char* duplicate = "este es un test de las commons\n";
    printf("%s", string_duplicate(duplicate));
}
