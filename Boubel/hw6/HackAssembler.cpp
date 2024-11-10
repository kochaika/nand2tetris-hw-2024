#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <regex>
#include <bitset>

struct Code {
    struct dest_table {
        std::map<std::string, std::string> mp;
        dest_table() {
            mp["M"] = "001";
            mp["D"] = "010";
            mp["MD"] = "011";
            mp["A"] = "100";
            mp["AM"] = "101";
            mp["AD"] = "110";
            mp["AMD"] = "111";
        };
        std::string get_code(std::string key) {
            auto iter = mp.find(key);
            if (iter != mp.end()) return iter->second;
            else return "000";
        }
    };

    dest_table dt = dest_table();

    struct jump_table {
        std::map<std::string, std::string> mp;
        jump_table() {
            mp["JGT"] = "001";
            mp["JEQ"] = "010";
            mp["JGE"] = "011";
            mp["JLT"] = "100";
            mp["JNE"] = "101";
            mp["JLE"] = "110";
            mp["JMP"] = "111";
        };
        std::string get_code(std::string key) {
            auto iter = mp.find(key);
            if (iter != mp.end()) return iter->second;
            else return "000";
        }
    };
    jump_table jt = jump_table();
    struct comp_table {
        std::map<std::string, std::string> mp;
        comp_table() {
            mp["0"] = "0101010";
            mp["1"] = "0111111";
            mp["-1"] = "0111010";
            mp["D"] = "0001100";
            mp["A"] = "0110000";
            mp["!D"] = "0001101";
            mp["!A"] = "0110001";
            mp["-D"] = "0001111";
            mp["-A"] = "0110011";
            mp["D+1"] = "0011111";
            mp["A+1"] = "0110111";
            mp["D-1"] = "0001110";
            mp["A-1"] = "0110010";
            mp["D+A"] = "0000010";
            mp["D-A"] = "0010011";
            mp["A-D"] = "0000111";
            mp["D&A"] = "0000000";
            mp["D|A"] = "0010101";
            mp["M"] = "1110000";
            mp["!M"] = "1110001";
            mp["-M"] = "1110011";
            mp["M+1"] = "1110111";
            mp["M-1"] = "1110010";
            mp["D+M"] = "1000010";
            mp["D-M"] = "1010011";
            mp["M-D"] = "1000111";
            mp["D&M"] = "1000000";
            mp["D|M"] = "1010101";
        };

        std::string get_code(std::string key) {
            auto iter = mp.find(key);
            if (iter != mp.end()) return iter->second;
            else return "0000000";
        }
    };

    comp_table ct = comp_table();
};

struct symbol_table {
    int reg = 16;
    std::map <std::string, int> mp;
    symbol_table(int reg = 16) {
      for (int i = 0;i <= 15;i++) {
          std::string key = "R" + std::to_string(i);
          mp[key] = i;
      }
      mp["SCREEN"] = 16384;
      mp["KBD"] = 24576;
      mp["SP"] = 0;
      mp["LCL"] = 1;
      mp["ARG"] = 2;
      mp["THIS"] = 3;
      mp["THAT"] = 4;
    };
    void add_code (std::string key, int val) {
        auto iter = mp.find(key);
        if (iter == mp.end()) {
            if (val < 0) val = reg, reg++;
            mp[key] = val;
        }
    }
    int get_code (std::string key) {
        auto iter = mp.find(key);
        if (iter != mp.end()) return iter->second;
        else return -1;
    }
};

struct parser {
    std::string cur_dest;
    std::string cur_comp;
    std::string cur_jump;
    std::string cur_c;
    parser() {
        cur_dest = "null";
        cur_jump = "null";
        cur_comp = "";
        cur_c = "";
    };
    std::string trim (std::string s) {
        auto start = s.begin();
        while (start != s.end()) {
            auto val = *start;
            if (std::isspace(val)) {
                start++;
            }
            else break;
        }
        auto end = s.end();
        do {
            end--;
        }
        while (std::distance(start, end) > 0 && std::isspace(*end));
        return {start, end + 1};
    }

    std::vector <std::string> split_by_c (std::string s, char c) {
        std::vector <std::string> ans;
        std::string token;
        std::istringstream stream(s);
        while (std::getline(stream, token, c)) {
            ans.push_back(token);
        }
        return ans;
    }
    void handle_jump (std::string s) {
        if (s.find('/') != std::string::npos) {
            s = s.substr(0, s.find('/'));
        }
        std::vector <std::string> sp = split_by_c(s, ';');
        if (sp.size() != 0) {
            cur_comp = trim(sp[0]);
        }
        if (sp.size() > 1) {
            cur_jump = trim(sp[1]);
        }
    }
    int parse (std::string s) {
        cur_dest = "null";
        cur_jump = "null";
        cur_comp = "";
        cur_c = "";
        auto iter = s.find("//");
        std::string prev_s = s;
        if (iter != std::string::npos) {
            s = s.substr(0, iter);
        }
        s = trim(s);
        if (s.size() != 0 && s[0] != '/') {
            iter = s.find('@');
            if (iter != std::string::npos) {
                cur_c = trim(s.substr(iter + 1));
            } else {
                iter = s.find('=');
                if (iter != std::string::npos) {
                    auto lf = split_by_c(s, '=');
                    cur_dest = trim(lf[0]);
                    if (lf.size() > 1) {
                        iter = lf[1].find(';');
                        if (iter != std::string::npos) {
                            handle_jump(lf[1]);
                        } else {
                            iter = lf[1].find('/');
                            if (iter != std::string::npos) {
                                lf[1] = lf[1].substr(0, iter);
                            }
                            cur_comp = trim(lf[1]);
                        }
                    }
                } else {
                    auto iter1 = s.find('+');
                    auto iter2 = s.find('-');
                    if (iter1 != std::string::npos || iter2 != std::string::npos) {
                        iter = s.find(';');
                        if (iter != std::string::npos) {
                            handle_jump(s);
                        } else {
                            iter = s.find('/');
                            if (iter != std::string::npos) {
                                s = s.substr(0, iter);
                            }
                            cur_comp = trim(s);
                        }
                    } else {
                        iter = s.find(';');
                        if (iter != std::string::npos) {
                            handle_jump(s);
                        } else {
                            iter = s.find('/');
                            if (iter != std::string::npos) {
                                s = s.substr(0, iter);
                            }
                            cur_jump = trim(s);
                        }
                    }
                }
            }
            return 1;
        }
        return -1;
    }
};

struct HackAssembler {
  symbol_table st;
  HackAssembler() {
      st = symbol_table();
  };
  void pass1 (std::string filename) {
      std::ifstream inp(filename);
      std::string line;
      int lline = 0;
      while (std::getline(inp, line)) {
          parser pr = parser();
          if (pr.parse(line) > 0) {
              std::string t = line;
              t.erase(0, t.find_first_not_of(" \t"));
              if (t.size() > 0 && t[0] == '(') {
                  std::string label = t.substr(1, t.find(')') - 1);
                  st.add_code(label, lline);
              } else lline++;
          }
          }
      inp.close();
      }
      void MachineCode (std::string filename) {
      std::ifstream inp(filename);
      std::string out_filename = filename.substr(0, filename.find('.') + 1) + "hack";
      std::ofstream out(out_filename);
      std::string line;
      int lline = 0;
      while (std::getline(inp, line)) {
          parser pr = parser();
          if (pr.parse(line) > 0) {
              std::string t = line;
              t.erase(0, t.find_first_not_of(" \t"));
              if (t.size() == 0) {
                  continue;
              }
              if (t[0] == '(') {
                  continue;
              }
              if (pr.cur_c.size() != 0) {
                  int val;
                  std::string c = pr.cur_c;
                  if (std::regex_match(c, std::regex("\\d+"))) {
                      val = std::atoi(c.c_str());
                  }
                  else {
                      st.add_code(c, -1);
                      val = st.get_code(c);
                  }
                  std::bitset<16> ans(val);
                  if (lline != 0) out << '\n';
                  out << ans.to_string();
                  lline++;
              }
              else {
                  Code cd = Code();
                  std::string dest = cd.dt.get_code(pr.cur_dest);
                  std::string comp = cd.ct.get_code(pr.cur_comp);
                  std::string jump = cd.jt.get_code(pr.cur_jump);
                  //std::cerr << pr.cur_dest << " " << pr.cur_comp << " " << pr.cur_jump << '\n';
                  //std::cerr << dest << " " << comp << " " << jump << '\n';
                  if (lline != 0) out << '\n';
                  out << 111 << comp << dest << jump;
                  lline++;
              }
          }
      }
  }
  void process (std::string filename) {
      pass1(filename);
      MachineCode(filename);
  }
};

int main() {
    HackAssembler HA = HackAssembler();
    HA.process("Add.asm");
    HA = HackAssembler();
    HA.process("Max.asm");
    HA = HackAssembler();
    HA.process("Pong.asm");
    HA = HackAssembler();
    HA.process("PongL.asm");
    HA = HackAssembler();
    HA.process("MaxL.asm");
    HA = HackAssembler();
    HA.process("Rect.asm");
    HA = HackAssembler();
    HA.process("RectL.asm");
    return 0;
}