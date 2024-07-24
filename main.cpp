/*
    Tên tác giả: Hoàng Trần Việt Khải và Hấu Trung Thành
    Chức năng chương trình: Demo Quản lý thư viện sách 
    - Cho phép thêm sách mới vào thư viện.
    - Liệt kê danh sách các sách có trong thư viện.
    - Cho phép tìm kiếm sách theo tiêu đề.
    - Quản lý mượn và trả sách.
    Ngày bắt đầu: 13/07/2024
    Ngày hoàn thành: 18/07/2024
    Phiên bản: 2.0
    Ghi chú: Chương trình sử dụng cấu trúc dữ liệu cây nhị phân tìm kiếm để quản lý sách.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <fstream> //sử dụng để thao tác với các file đầu vào và đầu ra trong C++
#include <sstream>  // thư viện để làm việc với chuỗi, cung cấp các lớp và hàm để xử lý chuỗi dưới dạng luồng dữ liệu.
#include <algorithm>
#include <functional> // Thư viện này cung cấp các đối tượng hàm và các công cụ để làm việc với các hàm

using namespace std;

// 1. Cấu trúc dữ liệu cho cuốn sách
struct Sach { // Lớp này đại diện cho thông tin của một cuốn sách, bao gồm các thuộc tính như mã sách, tiêu đề, tác giả, thể loại, năm xuất bản và số lượng còn trong kho.

    string ma_sach;
    string tieu_de;
    string tac_gia;
    string the_loai;
    int nam_xuat_ban;
    int so_luong_con;

    Sach(string i, string t, string a, string g, int y, int q) 
        : ma_sach(i), tieu_de(t), tac_gia(a), the_loai(g), nam_xuat_ban(y), so_luong_con(q) {}

    string chuyen_thanh_chuoi() const { // Phương thức này chuyển đổi thông tin của cuốn sách thành một chuỗi để dễ dàng lưu trữ hoặc hiển thị.
        stringstream ss;
        ss << ma_sach << "," << tieu_de << "," << tac_gia << "," << the_loai << "," << nam_xuat_ban << "," << so_luong_con;
        return ss.str();
    }
};

// 2. Cây nhị phân tìm kiếm để lưu trữ và tìm kiếm sách
struct NodeSach { // Lớp NodeSach đại diện cho một nút trong cây nhị phân, chứa con trỏ đến đối tượng sách và hai con trỏ trái phải.

    Sach* sach;
    NodeSach* trai;
    NodeSach* phai;

    NodeSach(Sach* s) : sach(s), trai(nullptr), phai(nullptr) {}
};

struct CaySach {

    CaySach() : goc(nullptr) {}

    void chen(Sach* sach) { // 'chen()' dùng để chèn một cuốn sách mới vào cây.
        goc = chen_de_quy(goc, sach);
    }

    Sach* tim_theo_ma_sach(const string& ma_sach) {
        return tim_theo_ma_sach_de_quy(goc, ma_sach);
    }

    Sach* tim_theo_tieu_de(const string& tieu_de) {
        return tim_theo_tieu_de_de_quy(goc, tieu_de);
    }

    void hien_thi_theo_thu_tu() {
        hien_thi_theo_thu_tu_de_quy(goc);
    }

    void luu_vao_tep(const string& ten_tep) const {
        ofstream tep(ten_tep);
        if (!tep.is_open()) {
            cerr << "Không thể mở file để ghi!" << endl; //Báo lỗi nếu không thể mở tệp để ghi và xuất thông điệp lỗi ra màn hình
            return;
        }

        function<void(NodeSach*)> luu_theo_thu_tu = [&](NodeSach* node) { // Định nghĩa một đối tượng hàm để lưu dữ liệu theo thứ tự
            if (node != nullptr) {
                luu_theo_thu_tu(node->trai);
                tep << node->sach->chuyen_thanh_chuoi() << endl;
                luu_theo_thu_tu(node->phai);
            }
        };

        luu_theo_thu_tu(goc);
        tep.close();
    }

    void tai_tu_tep(const string& ten_tep) {
        ifstream tep(ten_tep); //// Khởi tạo đối tượng ifstream để đọc từ tệp có tên là 'ten_tep'
        if (!tep.is_open()) {
            cerr << "Không thể mở file để đọc!" << endl;
            return;
        }

        string line;
        while (getline(tep, line)) {
            stringstream ss(line); // // Khởi tạo đối tượng stringstream từ chuỗi 'line' để thao tác với dữ liệu dưới dạng luồng
            string ma_sach, tieu_de, tac_gia, the_loai;
            int nam_xuat_ban, so_luong_con;
            getline(ss, ma_sach, ',');
            getline(ss, tieu_de, ',');
            getline(ss, tac_gia, ',');
            getline(ss, the_loai, ',');
            ss >> nam_xuat_ban;
            ss.ignore(1, ',');
            ss >> so_luong_con;

            chen(new Sach(ma_sach, tieu_de, tac_gia, the_loai, nam_xuat_ban, so_luong_con));
        }

        tep.close();
    }


    NodeSach* goc;

    NodeSach* chen_de_quy(NodeSach* node, Sach* sach) {
        if (node == nullptr) return new NodeSach(sach);
        if (sach->tieu_de < node->sach->tieu_de) {
            node->trai = chen_de_quy(node->trai, sach);
        } else {
            node->phai = chen_de_quy(node->phai, sach);
        }
        return node;
    }

    Sach* tim_theo_ma_sach_de_quy(NodeSach* node, const string& ma_sach) {
        if (node == nullptr || node->sach->ma_sach == ma_sach) return node ? node->sach : nullptr;
        Sach* trai_tim = tim_theo_ma_sach_de_quy(node->trai, ma_sach);
        if (trai_tim) return trai_tim;
        return tim_theo_ma_sach_de_quy(node->phai, ma_sach);
    }
    
    Sach* tim_theo_tieu_de_de_quy(NodeSach* node, const string& tieu_de) {
        if (node == nullptr || node->sach->tieu_de == tieu_de) return node ? node->sach : nullptr;
        if (tieu_de < node->sach->tieu_de) return tim_theo_tieu_de_de_quy(node->trai, tieu_de);
        return tim_theo_tieu_de_de_quy(node->phai, tieu_de);
    }

    void hien_thi_theo_thu_tu_de_quy(NodeSach* node) {
        if (node != nullptr) {
            hien_thi_theo_thu_tu_de_quy(node->trai);
            cout << node->sach->chuyen_thanh_chuoi() << endl;
            hien_thi_theo_thu_tu_de_quy(node->phai);
        }
    }
};

// 3. Danh sách liên kết để lưu trữ người mượn
struct NguoiMuon {

    string ma_nguoi_muon;
    string ten;
    string dia_chi;
    vector<Sach*> cac_sach_dang_muon;

    NguoiMuon(string i, string n, string a) : ma_nguoi_muon(i), ten(n), dia_chi(a) {}

    void muon_sach(Sach* sach) {
        cac_sach_dang_muon.push_back(sach); // Thêm đối tượng 'sach' vào cuối vector 'cac_sach_dang_muon'
    }

    void tra_sach(const string& ma_sach) {
        cac_sach_dang_muon.erase(remove_if(cac_sach_dang_muon.begin(), cac_sach_dang_muon.end(), 
            [&](Sach* s) { return s-> ma_sach == ma_sach; }), cac_sach_dang_muon.end());
    }

    void hien_thi_cac_sach_dang_muon() {
        for (auto& sach : cac_sach_dang_muon) { // Duyệt qua từng phần tử 'sach' trong vector 'cac_sach_dang_muon'
            cout << sach->chuyen_thanh_chuoi() << endl;
        }
    }
};

struct DanhSachNguoiMuon {

    void them_nguoi_muon(NguoiMuon* nguoi_muon) {
        danh_sach_nguoi_muon.push_back(nguoi_muon);
    }

    NguoiMuon* tim_kiem_theo_ma(const string& ma) {
        for (auto& nguoi_muon : danh_sach_nguoi_muon) {
            if (nguoi_muon->ma_nguoi_muon == ma) return nguoi_muon;
        }
        return nullptr;
    }

    void hien_thi_tat_ca_nguoi_muon() {
        for (auto& nguoi_muon : danh_sach_nguoi_muon) {
            cout << nguoi_muon->ma_nguoi_muon << ": " << nguoi_muon->ten << ", " << nguoi_muon->dia_chi << endl;
        }
    }

    void luu_vao_tep(const string& ten_tep) const {
        ofstream tep(ten_tep);
        if (!tep.is_open()) {
            cerr << "Không thể mở file để ghi!" << endl;
            return;
        }

        for (auto& nguoi_muon : danh_sach_nguoi_muon) {
            tep << nguoi_muon->ma_nguoi_muon << "," << nguoi_muon->ten << "," << nguoi_muon->dia_chi << endl;
        }

        tep.close();
    }

    void tai_tu_tep(const string& ten_tep) {
        ifstream tep(ten_tep);
        if (!tep.is_open()) {
            cerr << "Không thể mở file để đọc!" << endl;
            return;
        }

        string line;
        while (getline(tep, line)) {
            stringstream ss(line);
            string ma, ten, dia_chi;
            getline(ss, ma, ',');
            getline(ss, ten, ',');
            getline(ss, dia_chi, ',');

            them_nguoi_muon(new NguoiMuon(ma, ten, dia_chi));
        }

        tep.close();
    }


    vector<NguoiMuon*> danh_sach_nguoi_muon;
};

// 4. Ngăn xếp để quản lý lịch sử mượn/trả sách
struct LichSuMuonTra {

    void them_bang_ghi(const string& ma_nguoi_muon, const string& ma_sach, bool muon) {
        lich_su.push({ma_nguoi_muon, ma_sach, muon});
    }

    void hien_thi_lich_su() {
        while (!lich_su.empty()) {
            auto bang_ghi = lich_su.top();
            lich_su.pop();
            cout << (bang_ghi.muon ? "Muon" : "Tra") 
                 << " - Ma nguoi muon: " << bang_ghi.ma_nguoi_muon 
                 << ", Ma sach: " << bang_ghi.ma_sach << endl;
        }
    }


    struct BangGhi {
        string ma_nguoi_muon;
        string ma_sach;
        bool muon;
    };

    stack<BangGhi> lich_su;
};

// 5. Hàm hiển thị menu
void hien_thi_menu() {
    cout << "\n------------------MENU------------------\n";
    cout << "1. Thêm sách\n";
    cout << "2. Hiển thị danh sách sách\n";
    cout << "3. Thêm người mượn\n";
    cout << "4. Mượn sách\n";
    cout << "5. Trả sách\n";
    cout << "6. Hiển thị lịch sử mượn/trả sách\n";
    cout << "7. Hiển thị danh sách người mượn\n";
    cout << "8. Tìm sách\n";
    cout << "0. Thoát\n";
    cout << "\n----------------------------------------\n";
    cout << "Lựa chọn: ";
}

// 6. Hàm chính để chạy chương trình
int main() {
    // Khởi tạo dữ liệu
    CaySach cay_sach;
    DanhSachNguoiMuon danh_sach_nguoi_muon;
    LichSuMuonTra lich_su;


    // Nạp dữ liệu từ file
    cay_sach.tai_tu_tep("sach.txt");
    danh_sach_nguoi_muon.tai_tu_tep("nguoi_muon.txt");

    while (true) {
        hien_thi_menu();
        int lua_chon;
        cin >> lua_chon;
        cin.ignore();

        if (lua_chon == 0) break;

        switch (lua_chon) {
            case 1: {
                string ma_sach, tieu_de, tac_gia, the_loai;
                int nam_xuat_ban, so_luong;
                cout << "Nhap ma sach: ";
                getline(cin, ma_sach);
                cout << "Nhap tieu de: ";
                getline(cin, tieu_de);
                cout << "Nhap tac gia: ";
                getline(cin, tac_gia);
                cout << "Nhap the loai: ";
                getline(cin, the_loai);
                cout << "Nhap nam xuat ban: ";
                cin >> nam_xuat_ban;
                cout << "Nhap so luong: ";
                cin >> so_luong;
                cin.ignore();
                cay_sach.chen(new Sach(ma_sach, tieu_de, tac_gia, the_loai, nam_xuat_ban, so_luong));
                break;
            }
            case 2:
                cout << "Danh sach sach:" << endl;
                cay_sach.hien_thi_theo_thu_tu();
                break;
            case 3: {
                string ma, ten, dia_chi;
                cout << "Nhap ma nguoi muon: ";
                getline(cin, ma);
                cout << "Nhap ten: ";
                getline(cin, ten);
                cout << "Nhap dia chi: ";
                getline(cin, dia_chi);
                danh_sach_nguoi_muon.them_nguoi_muon(new NguoiMuon(ma, ten, dia_chi));
                break;
            }
            case 4: {
                string ma_nguoi_muon, tieu_de_sach;
                cout << "Nhap ma nguoi muon: ";
                getline(cin, ma_nguoi_muon);
                cout << "Nhap tieu de sach: ";
                getline(cin, tieu_de_sach);
                NguoiMuon* nguoi_muon = danh_sach_nguoi_muon.tim_kiem_theo_ma(ma_nguoi_muon);
                if (nguoi_muon) {
                    Sach* sach_muon = cay_sach.tim_theo_tieu_de(tieu_de_sach);
                    if (sach_muon && sach_muon->so_luong_con > 0) {
                        nguoi_muon->muon_sach(sach_muon);
                        sach_muon->so_luong_con--;
                        lich_su.them_bang_ghi(nguoi_muon->ma_nguoi_muon, sach_muon-> ma_sach, true);
                    } else {
                        cout << "Sach khong ton tai hoac da het so luong." << endl;
                    }
                } else {
                    cout << "Nguoi muon khong ton tai." << endl;
                }
                break;
            }
            case 5: {
                string ma_nguoi_muon, ma_sach;
                cout << "Nhap ma nguoi muon: ";
                getline(cin, ma_nguoi_muon);
                cout << "Nhap ma sach: ";
                getline(cin, ma_sach);
                NguoiMuon* nguoi_muon = danh_sach_nguoi_muon.tim_kiem_theo_ma(ma_nguoi_muon);
                if (nguoi_muon) {
                    nguoi_muon->tra_sach(ma_sach);
                    Sach* sach = cay_sach.tim_theo_tieu_de(ma_sach);
                    if (sach) sach->so_luong_con++;
                    lich_su.them_bang_ghi(nguoi_muon->ma_nguoi_muon, ma_sach, false);
                } else {
                    cout << "Nguoi muon khong ton tai." << endl;
                }
                break;
            }
            case 6:
                cout << "Lich su muon/tra sach:" << endl;
                lich_su.hien_thi_lich_su();
                break;
            case 7:
                cout << "Danh sach nguoi muon:" << endl;
                danh_sach_nguoi_muon.hien_thi_tat_ca_nguoi_muon();
                break;
            case 8: { 
                string lua_chon_tim_kiem;
                cout << "Tìm sách theo (1 - Mã sách, 2 - Tiêu đề): ";
                getline(cin, lua_chon_tim_kiem);

                if (lua_chon_tim_kiem == "1") {
                    string ma_sach;
                    cout << "Nhập mã sách cần tìm: ";
                    getline(cin, ma_sach);
                    Sach* sach_tim_thay = cay_sach.tim_theo_ma_sach(ma_sach);
                    if (sach_tim_thay) {
                        cout << "Sách đã tìm thấy:" << endl;
                        cout << sach_tim_thay->chuyen_thanh_chuoi() << endl;
                    } else {
                        cout << "Không tìm thấy sách có mã là " << ma_sach << endl;
                    }
                } else if (lua_chon_tim_kiem == "2") {
                    string tieu_de;
                    cout << "Nhập tiêu đề sách cần tìm: ";
                    getline(cin, tieu_de);
                    Sach* sach_tim_thay = cay_sach.tim_theo_tieu_de(tieu_de);
                    if (sach_tim_thay) {
                        cout << "Sách đã tìm thấy:" << endl;
                        cout << sach_tim_thay->chuyen_thanh_chuoi() << endl;
                    } else {
                        cout << "Không tìm thấy sách có tiêu đề là " << tieu_de << endl;
                    }
                } else {
                    cout << "Lựa chọn không hợp lệ." << endl;
                }
                break;
            }
            
            default:
                cout << "Lua chon khong hop le." << endl;
        }
    }

    // Luu du lieu vao tep khi thoat
    cay_sach.luu_vao_tep("sach.txt");
    danh_sach_nguoi_muon.luu_vao_tep("nguoi_muon.txt");

    return 0;
}

