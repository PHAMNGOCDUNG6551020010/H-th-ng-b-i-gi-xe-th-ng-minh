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
            waitingOTO.enqueue(v);
            cout << ">> Bãi OTO đầy, xe được thêm vào hàng chờ.\n";
        }
    }


    // ======= In vé xe ngay sau khi thêm =======
  cout << "\n+============================================================================+\n";
    cout << "|                              VÉ XE NHẬP BÃI                                |\n";
    cout << "+============================================================================+\n";
    cout << "| Biển số        | " << setw(20) << left << v.plate << "\n";
    cout << "| Loại xe        | " << setw(20) << left << v.type << "\n";
    cout << "| Mã vé          | " << setw(20) << left << v.id << "\n";
    cout << "| Thời gian vào  | " << setw(20) << left <<formatTime(v.inTime) << endl;
    cout << "+============================================================================+\n";
    cout << "|              💬  CẢM ƠN QUÝ KHÁCH                          \n";
    cout << "+============================================================================+\n\n";
    // Thêm vào bãi hoặc hàng chờ
    
}
// chức năng 2 
void xuatXe(Stack &parkingXM, Stack &parkingOTO, Queue &waitingXM, Queue &waitingOTO, 
    LichSuManager &history, long long &doanhThu) {
cout << "Xuất theo: 1. Stack (đỉnh)  2. Biển số (tìm kiếm) - Nhập 1 hoặc 2: ";
int mode;

while (true) {
    if (cin >> mode && (mode == 1 || mode == 2)) break;

    cout << "❌ Lựa chọn không hợp lệ. Vui lòng nhập lại (1 hoặc 2): ";
    cin.clear();
    cin.ignore(1024, '\n');
}

    string plate;
    if (mode == 2) {
        cout << "Nhập biển số (chính xác): ";
        cin >> plate;
    }

    ThongTin x;

    if (mode == 1) x = parkingXM.pop();
    else x = parkingXM.popByPlate(plate);

    if (x.id == -1) {
        if (mode == 1) x = parkingOTO.pop();
        else x = parkingOTO.popByPlate(plate);
    }

    if (x.id != -1) {
        time_t out = time(NULL);
        double diff = difftime(out, x.inTime);
        int hours = (int)(diff / 3600);
        int minutes = (int)((diff - hours * 3600) / 60);
        int seconds = (int)(diff - hours * 3600 - minutes * 60);

        double totalHours = diff / 3600.0;
       long long base;
if (x.type == "XM") {
    base = 5000;
} else {
    base = 10000;
}
        long long fee = base;
        if (totalHours > 1.0) {
            fee = base + (long long)((ceil(totalHours) - 1.0)) * base;
        }
        doanhThu += fee;

        // ======= In bảng thông tin xuất xe =======
      cout << "\n+===========================================================================+\n";
        cout << "|                          THÔNG TIN XUẤT XE                                |\n";
        cout << "+===========================================================================+\n";
        cout << "| Biển số        | " << setw(53) << left << x.plate << "                     \n";
        cout << "| Loại xe        | " << setw(53) << left << x.type << "                      \n";
        cout << "| Thời gian vào  | " << setw(53) << left <<  formatTime(x.inTime) << "       \n";
        cout << "| Thời gian ra   | " << setw(53) << left << formatTime(out) << "             \n";
        cout << "| Thời gian gửi  | " << setw(2) << hours << " giờ " 
             << setw(2) << minutes << " phút " 
             << setw(2) << seconds << " giây" 
             << setw(24) << " " << "                                                          \n";
        cout << "| Phí gửi xe     | " << setw(53) << left << (to_string(fee) + " VND") << "   \n";
        cout << "+===========================================================================+\n";
        cout << "|       💬  CẢM ƠN QUÝ KHÁCH — THƯỢNG LỘ BÌNH AN!                            |\n";
        cout << "+===========================================================================+\n\n";

        // ======= Lưu lịch sử =======
        LichSu h;
        h.id = x.id;
        h.plate = x.plate;
        h.type = x.type;
        h.inTime = x.inTime;
        h.outTime = out;
        h.fee = fee;
        history.addHistory(h);

        // ======= Cập nhật xe chờ =======
        if (x.type == "XM" && !waitingXM.isEmpty()) {
            ThongTin w = waitingXM.dequeue();
            parkingXM.push(w);
            cout << ">> Xe từ hàng chờ vào bãi XM: " << w.plate << "\n";
        }
        if (x.type == "OTO" && !waitingOTO.isEmpty()) {
            ThongTin w = waitingOTO.dequeue();
            parkingOTO.push(w);
            cout << ">> Xe từ hàng chờ vào bãi OTO: " << w.plate << "\n";
        }
    } else {
        cout << ">> Không tìm thấy xe để xuất.\n";
    }
}

 void timKiemXe(Stack &baiXeXM, Stack &baiXeOTO, Queue &hangChoXM, Queue &hangChoOTO, LichSuManager &lichSu) {
    int luaChon;
    cout << "\n=== CHỌN KIỂU TÌM KIẾM ===\n";
    cout << "1. Theo MÃ SỐ VÉ\n";
    cout << "2. Theo BIỂN SỐ XE\n";
    cout << "Nhập lựa chọn (1 hoặc 2): ";

    while (true) {
        cin >> luaChon;
        if (luaChon == 1 || luaChon == 2) break;
        cout << "Lựa chọn không hợp lệ. Nhập lại (1 hoặc 2): ";
        cin.clear();
        cin.ignore(1024, '\n');
    }

    Vehicle* danhSach[4] = { baiXeXM.top, baiXeOTO.top, hangChoXM.head, hangChoOTO.head };
    string tenViTri[4] = { "Bãi XM", "Bãi OTO", "Hàng chờ XM", "Hàng chờ OTO" };

    bool timThay = false;
    Vehicle* xeTimThay = NULL;

    // === TÌM CHÍNH XÁC ===
    if (luaChon == 1) {
        int maVe;
        cout << "Nhập mã vé: ";
        while (!(cin >> maVe)) {
            cout << "Mã vé không hợp lệ. Nhập lại: ";
            cin.clear();
            cin.ignore(1024, '\n');
        }

        for (int i = 0; i < 4 && !timThay; i++) {
            Vehicle* cur = danhSach[i];
            while (cur) {
                if (cur->info.id == maVe) {
                    xeTimThay = cur;
                    timThay = true;
                    cout << "\n✅ TÌM THẤY KẾT QUẢ CHÍNH XÁC:\n";
                    cout << "Mã vé: " << cur->info.id << " | Biển số: " << cur->info.plate
                         << " | Loại: " << cur->info.type << " | Vị trí: " << tenViTri[i]
                         << " | Giờ vào: " << formatTime(cur->info.inTime) << "\n";
                    break;
                }
                cur = cur->next;
            }
        }
    } else {
        string bienSo;
        cout << "Nhập biển số xe: ";
        cin >> bienSo;
        string bienSoChuan = toUpperStr(bienSo);

        for (int i = 0; i < 4 && !timThay; i++) {
            Vehicle* cur = danhSach[i];
            while (cur) {
                if (toUpperStr(cur->info.plate) == bienSoChuan) {
                    xeTimThay = cur;
                    timThay = true;
                    cout << "\n✅ TÌM THẤY KẾT QUẢ CHÍNH XÁC:\n";
                    cout << "Mã vé: " << cur->info.id << " | Biển số: " << cur->info.plate
                         << " | Loại: " << cur->info.type << " | Vị trí: " << tenViTri[i]
                         << " | Giờ vào: " << formatTime(cur->info.inTime) << "\n";
                    break;
                }
                cur = cur->next;
            }
        }
    }

    // === KHÔNG TÌM THẤY -> TÌM GẦN ĐÚNG ===
    if (!timThay) {
        cout << "\n⚠️  Không tìm thấy kết quả chính xác. Tiến hành tìm gần đúng...\n";
        string key;

        if (luaChon == 1) {  // Tìm gần đúng theo mã vé
            cout << "Nhập ký tự gợi nhớ trong mã vé: ";
            cin >> key;
        } else {              // Tìm gần đúng theo biển số
            cout << "Nhập ký tự gợi nhớ trong biển số: ";
            cin >> key;
            key = toUpperStr(key);
        }

        bool timThayGanDung = false;
        cout << "\n┌─────────────────────── KẾT QUẢ TÌM KIẾM GẦN ĐÚNG ───────────────────────┐\n";
        cout << "│ " << left << setw(4) << "ID" << " │ " << setw(14) << "BIỂN SỐ" << " │ "
             << setw(10) << "LOẠI" << " │ " << setw(15) << "VỊ TRÍ" << " │ "
             << setw(25) << "GIỜ VÀO" << " │\n";
        cout << "├──────────────────────────────────────────────────────────────────────────┤\n";

        for (int i = 0; i < 4; i++) {
            Vehicle* cur = danhSach[i];
            while (cur) {
                string soSanh = (luaChon == 1) ? to_string(cur->info.id) : toUpperStr(cur->info.plate);
                if (soSanh.find(key) != string::npos) {
                    timThayGanDung = true;
                    cout << "│ " << setw(4) << cur->info.id
                         << " │ " << setw(14) << cur->info.plate
                         << " │ " << setw(10) << cur->info.type
                         << " │ " << setw(15) << tenViTri[i]
                         << " │ " << setw(25) << formatTime(cur->info.inTime) << " │\n";
                }
                cur = cur->next;
            }
        }

        cout << "└──────────────────────────────────────────────────────────────────────────┘\n";

        if (!timThayGanDung) {
            cout << "\n>> Không tìm thấy kết quả gần đúng.\n";
            cout << ">> Kiểm tra trong lịch sử xe đã rời bãi:\n";
            lichSu.showAllHistory();
        }
    }
}

void showMenu() {
cout << "\n+=============================================================================================================================+\n";
cout << "|  1. Them xe vao bai (hien thi chi tiet + ve)  |  2. Xuat xe (theo stack / bien so)  |  3. Hien thi thong tin                |\n";
cout << "+-----------------------------------------------------------------------------------------------------------------------------+\n";
cout << "|  4. Tim kiem xe                               |  5. Quan ly lich su xe da ra        |  6. Thong ke tong quan                |\n";
cout << "+-----------------------------------------------------------------------------------------------------------------------------+\n";
cout << "|  0. Thoat chuong trinh                                                                                                     |\n";
cout << "+=============================================================================================================================+\n";
cout << "                                                → Nhap lua chon cua ban :  ";
}
int main() {
    vector<NhanVien> dsNV;
    NhanVien nv1; nv1.username = "admin"; nv1.password = "123"; nv1.name = "Pham Ngoc Hung";
    dsNV.push_back(nv1);
    NhanVien nv2; nv2.username = "admin"; nv2.password = "111"; nv2.name = "Pham Ngoc Dung ";
    dsNV.push_back(nv2);
    NhanVien nv3; nv3.username = "admin"; nv3.password = "222"; nv3.name = "Tran Trong Nguyen";
    dsNV.push_back(nv3);
    NhanVien nv4; nv4.username = "admin"; nv4.password = "333"; nv4.name = "Nguyen Thanh Binh";
    dsNV.push_back(nv4);
    string tenNV;
    if (!dangNhap3Lan(dsNV, tenNV)) return 0;

    int cap;
    cout << "Nhập sức chứa bãi xe (số nguyên > 0): ";
    while (!(cin >> cap) || cap <= 0) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "Giá trị không hợp lệ. Nhập lại sức chứa bãi xe: ";
    }

    Stack parkingXM(cap);
    Stack parkingOTO(cap );
    Queue waitingXM;
    Queue waitingOTO;
    LichSuManager history;
    long long doanhThu = 0;
    int idCounter = 1000;

    int choice = -1;
    do {
        showMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "Lựa chọn không hợp lệ. Nhập lại: ";
        }
         if (choice == 1) {
    themXe(parkingXM, parkingOTO, waitingXM, waitingOTO, idCounter);
 }
    else if (choice == 2) {
    xuatXe(parkingXM, parkingOTO, waitingXM, waitingOTO, history, doanhThu);
 }
  else if (choice == 3) {
            cout << "\n--- Bãi xe XM ---\n";
             parkingXM.display();
            cout << "\n--- Bãi xe OTO ---\n"; 
            parkingOTO.display();
            cout << "\n--- Sơ đồ bãi XM ---\n"; parkingXM.showSlots();
            cout << "\n--- Sơ đồ bãi OTO ---\n"; parkingOTO.showSlots();
              cout << "\n--- Hàng chờ XM ---\n"; waitingXM.display();
            cout << "\n--- Hàng chờ OTO ---\n"; waitingOTO.display();
        }
          else if (choice == 4) {
            timKiemXe(parkingXM, parkingOTO, waitingXM, waitingOTO, history);
          }
           else if (choice == 5) {
            cout << "1. Lịch sử tất cả\n2. Xe gửi lâu nhất\n3. Xe phí cao nhất\n4.Xe phí thấp nhất\n5.Tìm kiếm trong khoảng thời gian\n Chọn   ";
    
            int opt;
            while (!(cin >> opt) || opt < 1 || opt > 5) {
                cin.clear();
                cin.ignore(1024, '\n');
                cout << "Lựa chọn không hợp lệ. Nhập lại: ";
            }
if (opt == 1) history.showAllHistory();
else if (opt == 2) history.showLongestParked();
else if (opt == 3) history.showMaxFee();
else if (opt == 4) history.showMinFee();
else if (opt == 5) history.hienThiXeRaTheoKhoangGio();

        }
           else if (choice == 6) {
          thongKeTongQuan(parkingXM, parkingOTO, waitingXM, waitingOTO, doanhThu);
        }
         else if (choice == 0) {
            cout << "Thoát chương trình.\n";
        } else {
            cout << "Chọn sai, vui lòng chọn lại.\n";
        }
    } while (choice != 0);

    return 0;
}

