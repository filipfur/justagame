#include "cardgen.h"

int main(int argc, const char* argv[])
{
    std::unique_ptr<CardGen> app = std::make_unique<CardGen>();
    app->run();
    return 0;
}