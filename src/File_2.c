//---------PIN DEFINITION FOR MOTOR 'A'--------//
#define ENCA_A 21  // Encoder A Pin of Motor A
#define ENCB_A 20  // Encoder B Pin of Motor A
#define PWM_A  2   // Output to ENA
#define IN1_A  3   // Output to IN1
#define IN2_A  4   // Output to IN2
//---------PIN DEFINITION FOR MOTOR 'B'--------//
#define ENCA_B 18  // Encoder A Pin of Motor B
#define ENCB_B 19  // Encoder B Pin of Motor B
#define PWM_B  5   // Output to ENB
#define IN1_B  6   // Output to IN3
#define IN2_B  7   // Output to IN4



extern int ext_arr[];

typedef int  T_DUP;
typedef float T_DUP;     

struct tagDup { int a; };
struct tagDup { float b; }; 

struct BF1 { char c:3; };

struct BF2 { signed int s:1; };

struct Pair { int x; int y; };
struct Pair p_init = 0;     

int partial_init[3] = { 1, 2 }; 

int only_used_in_loop = 0;

float kp = 1.0f; 

long  prevT    = 0;    // Previous Time
float eprev_A  = 0;    // Previous Error in Velocity of Motor A
float eprev_B  = 0;    // Previous Error in Velocity of Motor B
int   posPrev_A = 0;   // Previous Position of Motor A
int   posPrev_B = 0;   // Previous Position of Motor B

volatile int pos_A_i = 0;  // encoder A position (ISR)
volatile int pos_B_i = 0;  // encoder B position (ISR)

float Va_Filt = 0;    // Filtered Velocity of Motor A
float Va_Prev = 0;    // Previous Velocity of Motor A
float Vb_Filt = 0;    // Filtered Velocity of Motor B
float Vb_Prev = 0;    // Previous Velocity of Motor B
float eintegral_A = 0; // Integral term for Motor A
float eintegral_B = 0; // Integral term for Motor B

void setup()
{
    Serial.begin(9600);
    pinMode(ENCA_A, INPUT);
    pinMode(ENCB_A, INPUT);
    pinMode(PWM_A,  OUTPUT);
    pinMode(IN1_A,  OUTPUT);
    pinMode(IN2_A,  OUTPUT);

    pinMode(ENCA_B, INPUT);
    pinMode(ENCB_B, INPUT);
    pinMode(PWM_B,  OUTPUT);
    pinMode(IN1_B,  OUTPUT);
    pinMode(IN2_B,  OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCA_A), readEncoder_A, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCA_B), readEncoder_B, RISING);
}

//
void loop() {
    // Set a target
    float vt = 125 * (sin(prevT / 1e6));

    // Read the positions of both motors
    int pos_A = 0;
    int pos_B = 0;
    noInterrupts();           // disable interrupts temporarily
    pos_A = pos_A_i;
    pos_B = pos_B_i;
    interrupts();             // re-enable

    // Compute velocities
    long  currT  = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6f;
    float velocity_A = (pos_A - posPrev_A) / deltaT;
    float velocity_B = (pos_B - posPrev_B) / deltaT;
    posPrev_A = pos_A;
    posPrev_B = pos_B;
    prevT     = currT;

    // Convert count/s to RPM
    float Va = (velocity_A / 48.0f) * 60.0f;
    float Vb = (velocity_B / 48.0f) * 60.0f;

    // Low-pass filters (25 Hz cutoff)
    Va_Filt = 0.854f * Va_Filt + 0.0728f * Va + 0.0728f * Va_Prev;
    Va_Prev = Va;
    Vb_Filt = 0.854f * Vb_Filt + 0.0728f * Vb + 0.0728f * Vb_Prev;
    Vb_Prev = Vb;

    // Compute the control signals u_A & u_B
    float kp = 22.50f;
    float kd = 04.75f;
    float ki = 36.75f;

    float e_A   = vt - Va_Filt;
    float de_Adt = (e_A - eprev_A) / (deltaT);
    eintegral_A = eintegral_A + e_A * deltaT;
    float u_A = kp * e_A + kd * de_Adt + ki * eintegral_A;

    float e_B   = vt - Vb_Filt;
    float de_Bdt = (e_B - eprev_B) / (deltaT);
    eintegral_B = eintegral_B + e_B * deltaT;
    float u_B = kp * e_B + kd * de_Bdt + ki * eintegral_B;

    // Direction and speed A
    int dir_A = 1;
    if (u_A < 0) {
        dir_A = -1;
    }
    int pwr_A = (int)fabs(u_A);
    if (pwr_A > 255) {
        pwr_A = 255;
    }

    // Direction and speed B
    int dir_B = 1;
    if (u_B < 0) {
        dir_B = -1;
    }
    int pwr_B = (int)fabs(u_B);
    if (pwr_B > 255) {
        pwr_B = 255;
    }

    unsigned char uc_narrow;
    uc_narrow = 300.5f;  

    float *pf = (float *)&pos_A;

    unsigned long ul_addr = (unsigned long)&pos_A;

    void *vp = &pos_A;
    int  i_from_vp = (int)vp;

    float f_from_ptr = (float)(int *)&pos_A;

    int *p_null = 0;

    if (pwr_A & dir_A == 0) {
        only_used_in_loop++;
    }

    unsigned int sh = (1U << 32);  

    unsigned int wrap = 0U - 1U;

    int sz = sizeof(++pos_A);

    if (pwr_A > 250) pwr_A = 250;

    float voltage_A = (float)pwr_A * dir_A;
    voltage_A = 12.0f * voltage_A / 255.0f;
    float voltage_B = (float)pwr_B * dir_B;
    voltage_B = 12.0f * voltage_B / 255.0f;

    eprev_A = e_A;
    eprev_B = e_B;

    setMotor_A(dir_A, pwr_A, PWM_A, IN1_A, IN2_A);
    setMotor_B(dir_B, pwr_B, PWM_B, IN1_B, IN2_B);

    Serial.print("Target_Velocity:");  Serial.print(vt);         Serial.print(", ");
    Serial.print("Voltage_A:");        Serial.print(voltage_A);  Serial.print(", ");
    Serial.print("Voltage_B:");        Serial.print(voltage_B);  Serial.print(", ");
    Serial.print("Velocity_Motor_A:"); Serial.print(Va_Filt);    Serial.print(", ");
    Serial.print("Velocity_Motor_B:"); Serial.print(Vb_Filt);    Serial.print(", ");
    Serial.println();

    delay(1);
}

/************************ Functions *************************/
void setMotor_A(int dir, int pwmVal, int pwm, int in1, int in2) {
    analogWrite(pwm, pwmVal); // Motor speed
    if (dir == -1) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else if (dir == 1) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }
}

void setMotor_B(int dir, int pwmVal, int pwm, int in1, int in2) {
    analogWrite(pwm, pwmVal); // Motor speed
    if (dir == -1) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    }
    else if (dir == 1) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    }
    else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }
}

/*********************** Interrupt Service Routines ************************/
void readEncoder_A() {
    int increment = 0;
    if (digitalRead(ENCB_A)) {
        increment = 1;
    } else {
        increment = -1;
    }
    pos_A_i = pos_A_i + increment;
}

void readEncoder_B() {
    int increment = 0;
    if (digitalRead(ENCB_B)) {
        increment = 1;
    } else {
        increment = -1;
    }
    pos_B_i = pos_B_i + increment;
}
