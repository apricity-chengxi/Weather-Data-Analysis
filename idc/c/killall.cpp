#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

// 停止数据中心后台服务程序
int main(int argc, char *argv[])
{
    string command1("killall -9 procctl");
    cout << "command1: " << command1 << endl;
    if (system(command1.c_str()) == 0)
        cout << "command1 ok" << endl;
    else
        cout << "command1 failed" << endl;
    

    string command2("killall gzipfiles crtsurfdata deletefiles");
    cout << "command2: " << command2 << endl;
    if (system(command2.c_str()) == 0)
        cout << "command2 ok" << endl;
    else
        cout << "command2 failed" << endl;

    string command3("sleep 3");
    cout << "command3: " << command3 << endl;
    if (system(command3.c_str()) == 0)
        cout << "command3 ok" << endl;
    else
        cout << "command3 failed" << endl;


    string command4("killall -9 gzipfiles crtsurfdata deletefiles");
    cout << "command4: " << command4 << endl;
    if (system(command4.c_str()) == 0)
        cout << "command4 ok" << endl;
    else
        cout << "command4 failed" << endl;

    return 0;
}