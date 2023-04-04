#include <utils/hello.h>
#include <persona.h>

int main(void) {
  hello_world();
    
    t_persona *messi = persona_new("Memoria", "Messi", 3);

    char* messi_str = persona_to_string(messi);
    puts(messi_str);
    free(messi_str);

    persona_free(messi);

return 0;
}

