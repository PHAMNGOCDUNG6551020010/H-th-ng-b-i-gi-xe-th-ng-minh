#include <iostream>
#include <string>
#include <ctime>
#include <cmath>  
#include <vector>
#include <iomanip>
#include <cctype>
using namespace std;

// ======= Cấu trúc dữ liệu =======
struct ThongTin {
    int id;
    string plate;
    string type;
    time_t inTime;
};
struct Vehicle {
    ThongTin info;
    Vehicle* next;
};

struct LichSu {
    int id;
    string plate;
    string type;
    time_t inTime;
    time_t outTime;
    long long fee;
};

struct NhanVien {
    string username;
    string password;
    string name;
};
// ======= Tiện ích =======
string formatTime(time_t t) {
    t += 7 * 3600;
    tm *ltm = gmtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%H:%M:%S %d/%m/%Y", ltm);
    return string(buf);
}

string toUpperStr(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}


// ======= STACK (Bãi xe) =======
class Stack {
public:
    Vehicle* top;
    int capacity;
    int size;

    Stack(int c);
    bool isEmpty();
    bool isFull();
    void push(ThongTin v);
    ThongTin pop();
    ThongTin popByPlate(string plate);
    void display();
    void showSlots();
};

Stack::Stack(int c) { 
top = NULL; 
size = 0; capacity = c;
}
bool Stack::isEmpty() {
    return top == NULL;
    }
bool Stack::isFull() {
    return size >= capacity;
    }

void Stack::push(ThongTin v) {
    if (isFull()) { 
     cout << ">> Bãi xe đã đầy, không thể thêm.\n";
      return;
        }
    Vehicle* p = new Vehicle();
    p->info = v;
    p->next = top;
    top = p;
    size++;
}

ThongTin Stack::pop() {
    ThongTin info;
    if (isEmpty()) { 
    info.id = -1;
    return info; 
    }
    Vehicle* p = top;
    info = p->info;
    top = top->next;
    delete p;
    size--;
    return info;
}

ThongTin Stack::popByPlate(string plate) {

    Vehicle* cur = top;
    Vehicle* prev = NULL;
    while (cur) {
        if (toUpperStr(cur->info.plate) == toUpperStr(plate)) {
            if (prev == NULL) top = cur->next;
            else prev->next = cur->next;
            ThongTin info = cur->info;
            delete cur;
            size--;
            return info;
        }
        prev = cur;
        cur = cur->next;
    }
    ThongTin info;
    info.id = -1;
    return info;
}
void Stack::display() {
    if (isEmpty()) { 
        cout << ">> Bãi xe trống.\n"; 
        return; 
    }

    cout << "\n┌──────────────────────────────────────────────────────────────────────────────┐\n";
    cout << "│ " << left << setw(4) << "ID"
         << " │ " << setw(16) << "BIỂN SỐ"
         << " │ " << setw(10) << "LOẠI"
         << " │ " << setw(25) << "GIỜ VÀO (HH:MM:SS DD/MM/YYYY)"
         << " │\n";
    cout << "├──────────────────────────────────────────────────────────────────────────────┤\n";

    Vehicle* cur = top;
    while (cur) {
        cout << "│ " << left << setw(4)  << cur->info.id
             << " │ " << setw(12) << cur->info.plate
             << " │ " << setw(8)  << cur->info.type
             << " │ " << setw(29) << formatTime(cur->info.inTime)
             << " │\n";
        cur = cur->next;
    }

    cout << "└──────────────────────────────────────────────────────────────────────────────┘\n";
}

void Stack::showSlots() {
    cout << "\n======= SƠ ĐỒ BÃI XE (X = đã dùng) =======\n";
    for (int i = 1; i <= capacity; i++) {
        if (i <= size) cout << "[X]";
        else cout << "[ ]";
        if (i % 10 == 0) cout << "\n";
        else cout << " ";
    }
    cout << "\n";
}

// ======= QUEUE (Hàng chờ) =======
class Queue {
public:
    Vehicle* head;
    Vehicle* tail;
    int size;

    Queue();
    void enqueue(ThongTin v);
    ThongTin dequeue();
    bool isEmpty();
    void display();
};

Queue::Queue() { 
head = tail = NULL; 
size = 0; 
}
bool Queue::isEmpty() {
    return head == NULL;
    }
void Queue::enqueue(ThongTin v) {
    Vehicle* p = new Vehicle();
    p->info = v;
    p->next = NULL;
    if (head==NULL) 
    head = tail = p;
    else {
    tail->next = p;  tail = p; 
    }
    size++;
}
ThongTin Queue::dequeue() {
    ThongTin info;
    if (isEmpty()) { 
      info.id = -1; 
    return info; 
    }
    Vehicle* p = head;
    info = p->info;
    head = head->next;
    if (head==NULL) tail = NULL;
    delete p;
    size--;
    return info;
}

void Queue::display() {
    if (isEmpty()) {
        cout << ">> Hàng chờ trống.\n"; return;
        }
    cout << "\n┌──────────────────────────────────────────────────────────────────────────────┐\n";
    cout << "│ " << left << setw(4) << "ID"
         << " │ " << setw(16) << "BIỂN SỐ"
         << " │ " << setw(10) << "LOẠI"
         << " │ " << setw(25) << "GIỜ VÀO (HH:MM:SS DD/MM/YYYY)"
         << " │\n";
    cout << "├──────────────────────────────────────────────────────────────────────────────┤\n";

 
    Vehicle* cur = head;
    while (cur) {
        cout << "│ " << left << setw(4)  << cur->info.id
             << " │ " << setw(12) << cur->info.plate
             << " │ " << setw(8)  << cur->info.type
             << " │ " << setw(29) << formatTime(cur->info.inTime)
             << " │\n";
        cur = cur->next;
    }
     cout << "├──────────────────────────────────────────────────────────────────────────────┤\n";
}




// ======= Đăng nhập tối đa 3 lần =======
bool dangNhap3Lan(vector<NhanVien>& ds, string& tenNV) {
    string user;
    string pass;
    for (int i = 0; i < 3; i++) {
        cout << "Tên đăng nhập: "; cin >> user;
        cout << "Mật khẩu     : "; cin >> pass;
        for (int j = 0; j < ds.size(); j++) {
            if (ds[j].username == user && ds[j].password == pass) {
                tenNV = ds[j].name;
                cout << ">> Đăng nhập thành công! Xin chào " << tenNV << "\n";
                return true;
            }
        }
        cout << ">> Sai tài khoản hoặc mật khẩu! Còn " << 2 - i << " lần thử.\n";
    }
cout << ">> Vượt quá số lần đăng nhập. Thoát.\n";
    return false;
}

// ======= Quản lý lịch sử 
class LichSuManager {
public:
    vector<LichSu> historyList;

    void addHistory(LichSu h);
    void showAllHistory();
    void hienThiXeRaTheoKhoangGio();
    void showLongestParked();
    void showMinFee();
    void showMaxFee();
    void showVehicleDetail(LichSu x);
};


void LichSuManager::addHistory(LichSu h) {
    historyList.push_back(h);
}

void LichSuManager::showVehicleDetail(LichSu x) {
    cout << "\n+==========================================+\n";
    cout << "|               CHI TIẾT XE                |\n";
    cout << "+==========================================+\n";
    cout << "| Mã vé   : " << x.id << "\n";
    cout << "| Biển số : " << x.plate << "\n";
    cout << "| Loại xe : " << x.type << "\n";
    cout << "| Giờ vào  : " << formatTime(x.inTime) << "\n";
    cout << "| Giờ ra   : " << formatTime(x.outTime) << "\n";
    cout << "| Phí gửi  : " << x.fee << "\n";
    cout << "+==========================================+\n";
}
// Hiển thị tất cả lịch sử
void LichSuManager::showAllHistory() {
    if (historyList.size() == 0) {
        cout << ">> Lịch sử trống!\n";
        return;
    }

    cout << "\n+=================================================================================+\n";
    cout << "|                              DANH SÁCH LỊCH SỬ XE                                |\n";
    cout << "+=================================================================================+\n";
    cout << "| " << left << setw(4) << "ID"
         << " │ " << setw(12) << "BIỂN SỐ"
         << " │ " << setw(8) << "LOẠI XE"
         << " │ " << setw(20) << "GIỜ VÀO"
         << " │ " << setw(20) << "GIỜ RA"
         << " │ " << setw(12) << "PHÍ"
         << " |\n";
    cout << "+---------------------------------------------------------------------------------+\n";

    for (int i = 0; i < historyList.size(); i++) {
        cout << "| " << left << setw(4) << historyList[i].id
             << " │ " << setw(12) << historyList[i].plate
             << " │ " << setw(8) << historyList[i].type
             << " │ " << setw(20) << formatTime(historyList[i].inTime)
             << " │ " << setw(20) << formatTime(historyList[i].outTime)
             << " │ " << setw(12) << historyList[i].fee
             << " |\n";
    }

    cout << "+=================================================================================+\n";
}
void LichSuManager::showLongestParked() {
    if (historyList.empty()) {
        cout << ">> Lịch sử trống!\n";
        return;
    }

    LichSu longest = historyList[0];
    double maxTime = difftime(longest.outTime, longest.inTime);

    for (int i = 1; i < historyList.size(); i++) {
        double t = difftime(historyList[i].outTime, historyList[i].inTime);
        if (t > maxTime) {
            maxTime = t;
            longest = historyList[i];
        }
    }

    cout << "\n>> Xe gửi lâu nhất:\n";
    showVehicleDetail(longest);
}

// Hiển thị xe ra theo khoảng giờ
void LichSuManager::hienThiXeRaTheoKhoangGio() {
    int gioBatDau, gioKetThuc;

    // Nhập giờ bắt đầu
    cout << "Nhập giờ bắt đầu (0-23): ";
    while (!(cin >> gioBatDau) || gioBatDau < 0 || gioBatDau > 23) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Giờ không hợp lệ. Nhập lại: ";
    }

    // Nhập giờ kết thúc
    cout << "Nhập giờ kết thúc (0-23): ";
    while (!(cin >> gioKetThuc) || gioKetThuc < 0 || gioKetThuc > 23) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Giờ không hợp lệ. Nhập lại: ";
    }

    if (gioKetThuc < gioBatDau) {
        cout << ">> Khoảng giờ không hợp lệ!\n";
        return;
    }


    cout << "\n+=========================================================================================================+\n";
    cout << "|                         DANH SÁCH XE RA TRONG KHOẢNG GIỜ                                                |\n";
    cout << "+=========================================================================================================+\n";
    cout << "| " << left << setw(4) << "ID" 
         << " │ " << setw(18) << "BIỂN SỐ" 
         << " │ " << setw(10) << "LOẠI XE" 
         << " │ " << setw(22) << "GIỜ VÀO" 
         << " │ " << setw(32) << "GIỜ RA" 
         << " │ " << setw(10) << "PHÍ" <<"    |"
         << " \n";
    cout << "+---------------------------------------------------------------------------------------------------------+\n";

    bool coXeTrongKhoangGio = false;
    for (int i = 0; i < historyList.size(); i++) {
        // Bỏ qua các xe chưa xuất
        if (historyList[i].outTime == 0) continue;

        // Cộng 7 giờ để lấy giờ Việt Nam
        time_t outTime = historyList[i].outTime + 7*3600;
        tm *tgRa = gmtime(&outTime);
        int gioRa = tgRa->tm_hour;

        // Nếu giờ ra nằm trong khoảng giờ nhập
        if (gioRa >= gioBatDau && gioRa <= gioKetThuc) {
            coXeTrongKhoangGio = true;

            time_t inTime = historyList[i].inTime + 7*3600;
            tm *tgVao = gmtime(&inTime);

            cout << "| " << left << setw(4) << historyList[i].id
                 << " │ " << setw(14) << historyList[i].plate
                 << " │ " << setw(9) << historyList[i].type
                 << " │ " << setw(15) << put_time(tgVao, "%H:%M:%S %d/%m/%Y")
                 << " │ " << setw(10) << put_time(tgRa, "%H:%M:%S %d/%m/%Y")
                 << " │ " << setw(10) << historyList[i].fee
                 << "|\n";
        }
    }

    cout << "+==========================================================================================================+\n";

    if (!coXeTrongKhoangGio) {
        cout << ">> Không có xe ra trong khoảng giờ " << gioBatDau << " - " << gioKetThuc << "\n";
    }
}


// Xe có phí thấp nhất
void LichSuManager::showMinFee() {
    if (historyList.size() == 0) {
        cout << ">> Lịch sử trống!\n";
        return;
    }

    LichSu minFeeXe = historyList[0];
    for (int i = 1; i < historyList.size(); i++) {
        if (historyList[i].fee < minFeeXe.fee) {
            minFeeXe = historyList[i];
        }
    }

    cout << "\n>> Xe có phí gửi thấp nhất:\n";
    showVehicleDetail(minFeeXe);
}

// Xe có phí cao nhất
void LichSuManager::showMaxFee() {
    if (historyList.size() == 0) {
        cout << ">> Lịch sử trống!\n";
        return;
    }

    LichSu maxFeeXe = historyList[0];
    for (int i = 1; i < historyList.size(); i++) {
        if (historyList[i].fee > maxFeeXe.fee) {
            maxFeeXe = historyList[i];
        }
    }

    cout << "\n>> Xe có phí gửi cao nhất:\n";
    showVehicleDetail(maxFeeXe);
}


// ======= Hiển thị chi tiết xe =======
void showVehicleDetail(ThongTin &x) {
    cout << "\n===== CHI TIẾT XE =====\n";
    cout << "Mã vé   : " << x.id << "\n";
    cout << "Biển số : " << x.plate << "\n";
    cout << "Loại xe : " << x.type << "\n";
    cout << "Giờ vào  : " << formatTime(x.inTime) << "\n";
}


// ======= Thống kê =======
void thongKeLoaiXe(Stack &xm, Stack &oto, Queue &qxm, Queue &qoto) {

    int xmc = xm.size + qxm.size;

    int otoc = oto.size + qoto.size;

     cout << "\nThống kê loại xe:\nXe máy: " << xmc << "\nÔ tô: " << otoc << "\n";






}

void thongKeTongQuan(Stack &xm, Stack &oto, Queue &qxm, Queue &qoto, long long doanhThu) {
    cout << "\n======= THỐNG KÊ TỔNG QUAN =======\n";
    cout << "Bãi xe XM: " << xm.size << "/" << xm.capacity << "\n";
    cout << "Bãi xe OTO: " << oto.size << "/" << oto.capacity << "\n";
    cout << "Hàng chờ XM: " << qxm.size << "\n";
    cout << "Hàng chờ OTO: " << qoto.size << "\n";
    cout << "Doanh thu: " << doanhThu << " VND\n";
}

// chức năng 1 
void themXe(Stack &parkingXM, Stack &parkingOTO, Queue &waitingXM, Queue &waitingOTO, int &idCounter) {
    ThongTin v;
    v.id = ++idCounter;

    // --- Nhập biển số ---
    while (true) {
        cout << "Nhập biển số (tối đa 8 ký tự): ";
        cin >> v.plate;
        if (v.plate.size() <= 8) break; // hợp lệ thì thoát vòng
        cout << "❌ Biển số quá dài! Vui lòng nhập lại.\n";
    }

    // --- Nhập loại xe ---
    while (true) {
        cout << "Loại xe (XM / OTO): ";
        cin >> v.type;
        v.type = toUpperStr(v.type);
        if (v.type == "XM" || v.type == "OTO") break; // hợp lệ thì thoát vòng
        cout << "❌ Loại xe không hợp lệ! Vui lòng nhập lại.\n";
    }

    // --- Sau khi hợp lệ mới cho vào bãi ---
    v.inTime = time(NULL);

    if (v.type == "XM") {
        if (!parkingXM.isFull()) {
            parkingXM.push(v);
            cout << ">> Xe vào bãi XM thành công.\n";
        } else {
            waitingXM.enqueue(v);
            cout << ">> Bãi XM đầy, xe được thêm vào hàng chờ.\n";
        }
    } else {
        if (!parkingOTO.isFull()) {
            parkingOTO.push(v);
            cout << ">> Xe vào bãi OTO thành công.\n";
        } else {
 

