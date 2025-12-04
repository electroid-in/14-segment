// ---------------------------
// 74HC595 pin connections
// ---------------------------
int DS   = 10;   // SER
int SHCP = 8;    // Shift Clock
int STCP = 9;    // Latch Clock

// ---------------------------------------------------------
// Segment bit mapping (your wiring)
// ---------------------------------------------------------
//
// SR1 (low byte):
// bit0 = DP
// bit1 = A
// bit2 = B
// bit3 = C
// bit4 = D
// bit5 = E
// bit6 = F
// bit7 = unused
//
// SR2 (high byte):
// bit0 = P
// bit1 = G
// bit2 = H
// bit3 = J
// bit4 = K
// bit5 = L
// bit6 = M
// bit7 = N
//
// ---------------------------------------------------------

struct Glyph {
  uint16_t val;   // SR2<<8 | SR1
};

// ---------------------------------------------------------
// DIGITS 0–9 HEX CODES
// ---------------------------------------------------------
Glyph numbers[10] = {
  {0x117E}, // 0 → ABCDEF P K
  {0x040C}, // 1 → H B C
  {0x8836}, // 2 → A B D E N J
  {0x081E}, // 3 → A B C D J
  {0x884C}, // 4 → F N J B C
  {0x885A}, // 5 → A F N J C D
  {0x887A}, // 6 → A F E D C N J
  {0x000E}, // 7 → A B C
  {0x887E}, // 8 → A F E D C B N J
  {0x885F}  // 9 → A B C D F N J
};

// ---------------------------------------------------------
// LETTERS A–Z HEX CODES
// ---------------------------------------------------------
Glyph letters[26] = {
  {0x886E}, // A → EFABCNJ
  {0x2A1E}, // B → ABCDLGJ (updated)
  {0x0072}, // C → AFED
  {0x221E}, // D → ABCDLG  (updated)
  {0x8072}, // E → AFEND
  {0x8062}, // F → AFEN
  {0x087A}, // G → AFEDCJ
  {0x886C}, // H → FENJBC
  {0x2212}, // I → AGLD
  {0x003C}, // J → BCDE
  {0x9460}, // K → FENHK
  {0x0070}, // L → FED
  {0x056C}, // M → EFPHBC
  {0x116C}, // N → EFPKCB
  {0x007E}, // O → ABCDEF
  {0x8966}, // P → EFPBJN
  {0x107E}, // Q → ABCDEFK
  {0x9866}, // R → EFABNJK
  {0x885A}, // S → AFNJCD
  {0x2202}, // T → AGL
  {0x007C}, // U → FEDCB
  {0x4460}, // V → FEMH
  {0x506C}, // W → FEBCMK
  {0x5500}, // X → PKMH
  {0x2500}, // Y → PHL
  {0x4412}  // Z → ADHM
};

// ---------------------------------------------------------
// Send a 16-bit glyph to the 74HC595 pair
// ---------------------------------------------------------
void sendToShiftRegisters(uint16_t pattern)
{
  byte sr1 = pattern & 0xFF;        // low byte
  byte sr2 = (pattern >> 8) & 0xFF; // high byte

  // Common-anode → LOW = ON → invert bits
  sr1 = ~sr1;
  sr2 = ~sr2;

  digitalWrite(STCP, LOW);
  shiftOut(DS, SHCP, MSBFIRST, sr2); // high byte first
  shiftOut(DS, SHCP, MSBFIRST, sr1); // then low byte
  digitalWrite(STCP, HIGH);
}

// ---------------------------------------------------------
// Display a letter A–Z
// ---------------------------------------------------------
void displayLetter(char c)
{
  if (c >= 'a' && c <= 'z')
      c -= 32;  // convert lowercase to uppercase

  if (c < 'A' || c > 'Z')
      return;   // ignore invalid characters

  uint16_t pattern = letters[c - 'A'].val;
  sendToShiftRegisters(pattern);
}

// ---------------------------------------------------------
// Display a digit 0–9
// ---------------------------------------------------------
void displayNumber(int n)
{
  if (n < 0 || n > 9) return;

  uint16_t pattern = numbers[n].val;
  sendToShiftRegisters(pattern);
}

// ---------------------------
// Setup
// ---------------------------
void setup() {
  pinMode(DS, OUTPUT);
  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
}

// ---------------------------
// Loop — cycle letters and numbers
// ---------------------------
void loop() {
  // Cycle numbers 0–9
  for (int n = 0; n <= 9; n++) {
    displayNumber(n);
    delay(1000);
  }

  // Cycle letters A–Z
  for (char c = 'A'; c <= 'Z'; c++) {
    displayLetter(c);
    delay(1000);
  }
}

