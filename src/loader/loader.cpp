#include "loader.h"

#include "body/body-entry.h"


namespace myapp {

int load(int argc, char *argv[])
{
    return body_entry(argc, argv);
}

}
