#include <iomanip>      // setw, setfill
#include <fstream>      // fstream
#include <termios.h>    // struct termios, tcgetattr(), tcsetattr()
#include <stdio.h>      // perror(), stderr, stdin, fileno()

using namespace std;

class KeyReader {
public:
    static int startKeyListener(std::function<void(char)> keyPressedListener)
    {
        struct termios t;
        struct termios t_saved;

        // Set terminal to single character mode.
        tcgetattr(fileno(stdin), &t);
        t_saved = t;
        t.c_lflag &= (~ICANON & ~ECHO);
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        if (tcsetattr(fileno(stdin), TCSANOW, &t) < 0) {
            perror("Unable to set terminal to single character mode");
            return -1;
        }

        // Read single characters from cin.
        std::streambuf *pbuf = cin.rdbuf();
        bool done = false;
        while (!done) {
            //cout << "Enter an character (or esc to quit): " << endl;
            char c;
            if (pbuf->sgetc() == EOF) done = true;
            c = pbuf->sbumpc();
            if (c == 0x1b) {
                cout << "You entered character " << (c+0) << "0x" << setw(2) << setfill('0') << hex << int(c) << "'" << endl;
                done = true;
            } else {
                keyPressedListener(c);
            }
        }

        // Restore terminal mode.
        if (tcsetattr(fileno(stdin), TCSANOW, &t_saved) < 0) {
            perror("Unable to restore terminal mode");
            return -1;
        }
        return 0;
    }
};
