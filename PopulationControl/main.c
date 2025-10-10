#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "service.h"
#include "tests.h"

int main() {
    //runAllTests();

    DynamicArray* repo = createDynamicArray(10);
    Service* service = CreateService(repo);

    start(service);

    return 0;
}