#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask for name then print it
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}