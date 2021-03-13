#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>

int main(int argc, char const *argv[])
{
    // lcd_init();

    // printInScreen("Teste 1", "Teste 2");

    read_from_bme();

    return 0;
}
