
//####################  Elliptical hip trajectory  ###############################//

//////////////////////////////////  Library and variables  ////////////////////////////////////////////
#include <Servo.h>
Servo myservo[10];

double w = 9.0;
double a = 8.3;
double b = 7.7;
double c = 6.3;
double d = 4.5;
double e = 1.5;
double H = 28.3; // total height of the Robot
double S = 25.0; // siting height of the Robot
double h = 28.3; // current height of the Robot

double X0 = 15; // length of a full step
double Y0 = 0.54*w; // amount of left/right shift of hip during walking
double Z0 = 3.0; // full leg up length during walking

double t0 = 1.0; // full time to complete single  action

double PhipCenter[3] = {0, 0, 0};
double PfootL[3] = {0, 0, 0};
double PfootR[3] = {0, 0, 0};

double NormalMotorPosition[] = {94, 90, 83, 90, 84, 82, 97, 112, 90, 88};
double currentMotorPosition[10] = {94, 90, 83, 90, 84, 82, 97, 112, 90, 88};
double FinalMotorPosition[10] = {};

///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Setup your code here  /////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  for(int i=0; i<10; i++){
    myservo[i].attach(i+2);
  }

  setMotors(NormalMotorPosition);
  StandAtHeight(25);
  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Run your code here  ///////////////////////////////////////////////
void loop() {
  
  StartByLeftLeg();
  
  WalkByRightLeg();
  WalkByLeftLeg();
  WalkByRightLeg();
  WalkByLeftLeg();
  WalkByRightLeg();
  WalkByLeftLeg();
  
  StopByRightLeg();
  delay(100000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Generate actions  /////////////////////////////////////////////////

void StandAtHeight(double height){
  double t=0.0;
  while(t<=t0/4){
    StandingAtHeight(t, height, PhipCenter, PfootL, PfootR);
    CalculateMotorPosistion(PhipCenter, PfootL, PfootR, FinalMotorPosition);
    setMotors(FinalMotorPosition);
    t+=0.05;
  }
  h = height;
}

void StartByLeftLeg(){
  double t=0.0;
  while(t<=t0){
    LeftLegStarting(t, PhipCenter, PfootL, PfootR);
    CalculateMotorPosistion(PhipCenter, PfootL, PfootR, FinalMotorPosition);
    setMotors(FinalMotorPosition);
    t+=0.05;
  }
}

void WalkByRightLeg(){
  double t=0.0;
  while(t<=t0){
    RightLegWalking(t, PhipCenter, PfootL, PfootR);
   CalculateMotorPosistion(PhipCenter, PfootL, PfootR, FinalMotorPosition);
    setMotors(FinalMotorPosition);
    t+=0.05;
  }
}

void WalkByLeftLeg(){
  double t=0.0;
  while(t<=t0){
    LeftLegWalking(t, PhipCenter, PfootL, PfootR);
    Serial.println(String(PhipCenter[0]) + " " + String(PhipCenter[1]) + " " + String(PhipCenter[2]) );
    Serial.println(String(PfootL[0]) + " " + String(PfootL[1]) + " " + String(PfootL[2]) );
    Serial.println(String(PfootR[0]) + " " + String(PfootR[1]) + " " + String(PfootR[2]) );
    Serial.println();
   CalculateMotorPosistion(PhipCenter, PfootL, PfootR, FinalMotorPosition);
    setMotors(FinalMotorPosition);
    t+=0.05;
  }
}

void StopByRightLeg(){
  double t=0.0;
  while(t<=t0+0.01){
    RightLegStoping(t, PhipCenter, PfootL, PfootR);
    CalculateMotorPosistion(PhipCenter, PfootL, PfootR, FinalMotorPosition);
    setMotors(FinalMotorPosition);
    t+=0.05;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Generate trajectories  ////////////////////////////////////////////

void StandingAtHeight(double t, double height, double (&PhipCenter)[3], double (&PfootL)[3], double (&PfootR)[3]){
  double z = h - (3.0*(h - height)*pow(t, 2))/pow(t0/4, 2) + (2.0*(h - height)*pow(t, 3))/pow(t0/4, 3);
  
  PhipCenter[0] = 0.0;
  PhipCenter[1] = 0.0;
  PhipCenter[2] = 0.0;

  PfootL[0] = PhipCenter[0];
  PfootL[1] = PhipCenter[1]-w/2;
  PfootL[2] = z;

  PfootR[0] = PhipCenter[0];
  PfootR[1] = PhipCenter[1]+w/2;
  PfootR[2] = z;
}

void LeftLegStarting(double t, double (&PhipCenter)[3], double (&PfootL)[3], double (&PfootR)[3]){
  double t1 = 0.3*t0;
  double t2 = t0-t1;

  if(t<t1){
    PhipCenter[0] = 0.0;
    PhipCenter[1] = (3.0*(Y0)*pow(t, 2))/pow(t1, 2) - (2.0*(Y0)*pow(t, 3))/pow(t1, 3);
  } else if(t<t2){
    PhipCenter[0] = 0.0;
    PhipCenter[1] = Y0;
  } else {
    PhipCenter[0] = (3.0*(X0/4)*pow((t-t2), 2))/pow((t0-t2), 2) - (2.0*(X0/4)*pow((t-t2), 3))/pow((t0-t2), 3);
    PhipCenter[1] = Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  }
  PhipCenter[2] = 0.0;

  t1 = 0.25*t0;
  t2 = 0.35*t0;
  double t3 = 0.7*t0;

  if(t<t1){
    PfootL[0] = 0.0;
    PfootL[2] = h;
  } else if(t<t2){
    PfootL[0] = 0.0;
    PfootL[2] = h - (3.0*(Z0)*pow((t-t1), 2))/pow((t2-t1), 2) + (2.0*(Z0)*pow((t-t1), 3))/pow((t2-t1), 3);
  } else if(t<t3){
    PfootL[0] = (3.0*(X0/2)*pow((t-t2), 2))/pow((t3-t2), 2) - (2.0*(X0/2)*pow((t-t2), 3))/pow((t3-t2), 3);
    PfootL[2] = h - Z0*sqrt(abs(1 - pow((2*PfootL[0]/X0), 2)));
  } else {
    PfootL[0] = X0/2;
    PfootL[2] = h;
  }
  PfootL[1] = -w/2;

  PfootR[0] = 0.0;
  PfootR[1] = w/2;
  PfootR[2] = h;
}

void RightLegWalking(double t, double (&PhipCenter)[3], double (&PfootL)[3], double (&PfootR)[3]){
  double t1 = 0.3*t0;
  double t2 = t0-t1;

  if(t<t1){
    PhipCenter[0] = -X0/4 + (3.0*(X0/4)*pow((t), 2))/pow((t1), 2) - (2.0*(X0/4)*pow((t), 3))/pow((t1), 3);
    PhipCenter[1] = -Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  } else if(t<t2){
    PhipCenter[0] = 0.0;
    PhipCenter[1] = -Y0;
  } else {
    PhipCenter[0] = (3.0*(X0/4)*pow((t-t2), 2))/pow((t0-t2), 2) - (2.0*(X0/4)*pow((t-t2), 3))/pow((t0-t2), 3);
    PhipCenter[1] = -Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  }
  PhipCenter[2] = 0.0;

  t1 = 0.25*t0;
  t2 = 0.75*t0;

  if(t<t1){
    PfootR[0] = -X0/2;
    PfootR[2] = h;
  } else if(t<t2){
    PfootR[0] = -X0/2 + (3.0*(X0)*pow((t-t1), 2))/pow((t2-t1), 2) - (2.0*(X0)*pow((t-t1), 3))/pow((t2-t1), 3);
    PfootR[2] = h - Z0*sqrt(abs(1 - pow((2*PfootR[0]/X0), 2)));
  } else {
    PfootR[0] = X0/2;
    PfootR[2] = h;
  }
  PfootR[1] = w/2;

  PfootL[0] = 0.0;
  PfootL[1] = -w/2;
  PfootL[2] = h;
}

void LeftLegWalking(double t, double (&PhipCenter)[3], double (&PfootL)[3], double (&PfootR)[3]){
  double t1 = 0.3*t0;
  double t2 = t0-t1;

  if(t<t1){
    PhipCenter[0] = -X0/4 + (3.0*(X0/4)*pow((t), 2))/pow((t1), 2) - (2.0*(X0/4)*pow((t), 3))/pow((t1), 3);
    PhipCenter[1] = Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  } else if(t<t2){
    PhipCenter[0] = 0.0;
    PhipCenter[1] = Y0;
  } else {
    PhipCenter[0] = (3.0*(X0/4)*pow((t-t2), 2))/pow((t0-t2), 2) - (2.0*(X0/4)*pow((t-t2), 3))/pow((t0-t2), 3);
    PhipCenter[1] = Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  }
  PhipCenter[2] = 0.0;

  t1 = 0.25*t0;
  t2 = 0.75*t0;

  if(t<t1){
    PfootL[0] = -X0/2;
    PfootL[2] = h;
  } else if(t<t2){
    PfootL[0] = -X0/2 + (3.0*(X0)*pow((t-t1), 2))/pow((t2-t1), 2) - (2.0*(X0)*pow((t-t1), 3))/pow((t2-t1), 3);
    PfootL[2] = h - Z0*sqrt(abs(1 - pow((2*PfootL[0]/X0), 2)));
  } else {
    PfootL[0] = X0/2;
    PfootL[2] = h;
  }
  PfootL[1] = -w/2;

  PfootR[0] = 0.0;
  PfootR[1] = w/2;
  PfootR[2] = h;
}

void RightLegStoping(double t, double (&PhipCenter)[3], double (&PfootL)[3], double (&PfootR)[3]){
  double t1 = 0.3*t0;
  double t2 = t0-t1;

  if(t<t1){
    PhipCenter[0] = -X0/4 + (3.0*(X0/4)*pow((t), 2))/pow((t1), 2) - (2.0*(X0/4)*pow((t), 3))/pow((t1), 3);
    PhipCenter[1] = -Y0*sqrt(abs(1 - pow((4*PhipCenter[0]/X0), 2)));
  } else if(t<t2){
    PhipCenter[0] = 0.0;
    PhipCenter[1] = -Y0;
  } else {
    PhipCenter[0] = 0.0;
    PhipCenter[1] = -Y0 + (3.0*(Y0)*pow((t-t2), 2))/pow((t0-t2), 2) - (2.0*(Y0)*pow((t-t2), 3))/pow((t0-t2), 3);
  }
  PhipCenter[2] = 0.0;

  t1 = 0.30*t0;
  t2 = 0.65*t0;
  double t3 = 0.75*t0;

  if(t<t1){
    PfootR[0] = -X0/2;
    PfootR[2] = h;
  } else if(t<t2){
    PfootR[0] = -X0/2 + (3.0*(X0/2)*pow((t-t1), 2))/pow((t2-t1), 2) - (2.0*(X0/2)*pow((t-t1), 3))/pow((t2-t1), 3);
    PfootR[2] = h - Z0*sqrt(abs(1 - pow((2*PfootR[0]/X0), 2)));
  } else if(t<t3){
    PfootR[0] = 0.0;
    PfootR[2] = h - Z0 + (3.0*(Z0)*pow((t-t2), 2))/pow((t3-t2), 2) - (2.0*(Z0)*pow((t-t2), 3))/pow((t3-t2), 3);
  } else {
    PfootR[0] = 0.0;
    PfootR[2] = h;
  }
  PfootR[1] = w/2;

  PfootL[0] = 0.0;
  PfootL[1] = -w/2;
  PfootL[2] = h;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Calculate motor position  /////////////////////////////////////////
void CalculateMotorPosistion(double PhipCenter[], double PfootL[], double PfootR[], double (&FinalMotorPosition)[10]){
  double q[5] = {0, 0, 0, 0, 0};
  
  double PhipL[3] = {PhipCenter[0], PhipCenter[1]-w/2, PhipCenter[2]};
  InverseKinematics(PhipL, PfootL, q);
  for(int i=0; i<5; i++){
    FinalMotorPosition[i] = NormalMotorPosition[i] + q[i]*(180.0/3.14);
  }
  
  double PhipR[3] = {PhipCenter[0], PhipCenter[1]+w/2, PhipCenter[2]};
  InverseKinematics(PhipR, PfootR, q);
  for(int i=0; i<5; i++){
    FinalMotorPosition[5+i] = NormalMotorPosition[5+i] - q[i]*(180.0/3.14);
  }
  FinalMotorPosition[5] = NormalMotorPosition[5] + q[0]*(180.0/3.14);
  FinalMotorPosition[9] = NormalMotorPosition[9] + q[4]*(180.0/3.14);
}

void InverseKinematics(double Phip[], double Pfoot[], double (&q)[5]){
  double x = Pfoot[0] - Phip[0];
  double y = Pfoot[1] - Phip[1];
  double z = Pfoot[2] - Phip[2];

  q[0] = -atan(y/(z-e));
  q[4] = q[0];

  double a1 = a*cos(q[0]);
  double b1 = b*cos(q[0]);
  double c1 = c*cos(q[0]);
  double d1 = d*cos(q[0]);
  double z1 = z - a1 - d1 - e;
  double r = sqrt(x*x + z1*z1);

  double phi1 = atan(x/z1);
  double phi2 = acos((b1*b1 + r*r - c1*c1)/(2*b1*r));
  q[3] = phi1 + phi2;

  double phi3 = acos((b1*b1 + c1*c1 - r*r)/(2*b1*c1));
  q[2] = - phi3 + 3.14;

  q[1] = -q[3] + q[2];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////  Set motor position  ///////////////////////////////////////////////

void setMotors(double finalMotorPosition[]){
  for(int t=0; t<=100; t++){
    for(int i=0; i<10; i++){
      int angle = currentMotorPosition[i] + t*(finalMotorPosition[i] - currentMotorPosition[i])/100;
      myservo[i].write(angle);
    }
    delay(2);
  }
  for(int i=0; i<10; i++){
    currentMotorPosition[i] = finalMotorPosition[i];
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
