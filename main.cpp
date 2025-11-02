#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int getchNonblock() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, flags);

    if (ch != EOF) return ch;
    return 0;
}

int main() {
    std::system("clear");

    std::srand(std::time(nullptr));
    std::cout << "=== MINI JACKPOT ===\n";
    std::cout << "Press ENTER to stop each reel!\n";
    std::cout << "====================\n\n";

    int reels[3] = {0, 0, 0};
    bool stopped[3] = {false, false, false};
    int stoppedCount = 0;

    std::cout << "\r[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]" << std::flush;

    while (stoppedCount < 3) {
        for (int i = 0; i < 3; i++) {
            if (!stopped[i])
                reels[i] = std::rand() % 10;
        }

        std::cout << "\r[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]" << std::flush;

        int ch = getchNonblock();
        if (ch == '\n') {
            if (stoppedCount < 3)
                stopped[stoppedCount++] = true;

            std::system("clear");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\n=========================\n";
    std::cout << "Result: [ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]\n";
    std::cout << "=========================\n";

    if (reels[0] == reels[1] && reels[1] == reels[2]) {
        std::cout << "💰💰💰 JACKPOT!!! MEGA BIG WIN! 💰💰💰\n";
    }

    std::cout << "Game over.\n";
    return 0;
}