
file = load('dataDAEMON.txt');

Px = file(:,1);
Py = file(:,2);
Pr = file(:,3);

Pxin = file(:,4);
Pyin = file(:,5);
Prin = file(:,6);

Vx = file(:,7);
Vy = file(:,8);
Vr = file(:,9);

CurSec = file(:,10);
GoalSec = file(:,11);
GoalSecR = file(:,12);

Vfl = file(:,13);
Vfr = file(:,14);
Vbr = file(:,15);
Vbl = file(:,16);

Pfl = file(:,17)*0.005;
Pfr = file(:,18)*0.005;
Pbr = file(:,19)*0.005;
Pbl = file(:,20)*0.005;

Rfl = file(:,21);
Rfr = file(:,22);
Rbr = file(:,23);
Rbl = file(:,24);

ENC_fl = file(:,25);
ENC_fr = file(:,26);
ENC_br = file(:,27);
ENC_bl = file(:,28);

figure
hold on;
plot(Pr,'.');
plot(Prin,'.');
plot(Vr,'.');
legend('Pr','Prin','Vr');

figure
hold on;
plot(Vfl,'.');
plot(Pfl,'.');
legend('Vfl','Pfl');

% figure
% hold on;
% plot(ENC_fl-ENC_fr);
% % plot(ENC_fr);
% figure
% plot(ENC_br-ENC_bl);
% % plot(ENC_bl);
% legend('fl','fr','br','bl');

figure
hold on;
plot(ENC_fl,'.');
plot(Pfl,'.');


% figure
% hold on;
% plot(CurSec,Px,'.');
% plot(CurSec,Pxin,'.');
% plot(CurSec,Vx,'.');
% legend('Px','Pxin','Vx','t');
% title('robot');
% 
% figure
% hold on;
% plot(CurSec,Vfl,'.');
% plot(CurSec,Pfl,'.');
% legend('Vfl','Pfl');
% title('wheel1');