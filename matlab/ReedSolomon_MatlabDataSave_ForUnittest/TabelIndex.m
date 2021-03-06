%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Masterthesis
% Zurcher Hochschule fur Angewandte Wissenschaften
% Zentrum fur Signalverarbeitung und Nachrichtentechnik
% (c) Michael Hoin
% 12.4.2011 ZSN
% info.zsn@zhaw.ch
%
% Code modified/extended (to DAB+ and Modes 2,3,4) by:
% (c) Tomasz Zielinski
% AGH University of Science and Technology
% Telecommunications Department
% Krakow, Poland
% 15.03.2015
% tzielin@agh.edu.pl
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% -----------------------------------------------------------------------------------------
% Tabel 6 with Subchannel Size, Protection Level and Bit Rate
% Standard ETSI EN 300 401, page 50
% -----------------------------------------------------------------------------------------
function [SubChannelSize, ProtLevel, BitRate] = TabelIndex(TabelIndexNr)

TabelIndexNr = TabelIndexNr + 1;

Tabel  = [ 16 5 32;
           21 4 32;
           24 3 32;
           29 2 32;
           35 1 32;
           24 5 48;
           29 4 48;
           35 3 48;
           42 2 48;
           52 1 48;
           29 5 56;
           35 4 56;
           42 3 56;
           52 2 56;
           32 5 64;
           42 4 64;
           48 3 64;
           58 2 64;
           70 1 64;
           40 5 80;
           52 4 80;
           58 3 80;
           70 2 80;
           84 1 80;
           48 5 96;
           58 4 96;
           70 3 96
           84 2 96;
           104 1 96;
           58 5 112;
           70 4 112;
           84 3 112;
           104 2 112;
           64 5 128;
           84 4 128;
           96 3 128;
           116 2 128;
           140 1 128;
           80 5 160;
           104 4 160;
           116 3 160;
           140 2 160;
           168 1 160;
           96 5 192;
           116 4 192
           140 3 192;
           168 2 192;
           208 1 192;
           116 5 224;
           140 4 224
           168 3 224;
           208 2 224;
           232 1 224;
           128 5 256;
           168 4 256;
           192 3 256;
           232 2 256;
           280 1 256;
           160 5 320;
           208 4 320;
           280 2 320;
           192 5 384;
           280 3 384;
           416 1 384];
       
SubChannelSize = Tabel(TabelIndexNr,1);
ProtLevel = Tabel(TabelIndexNr,2);
BitRate = Tabel(TabelIndexNr,3);
%disp([SubChannelSize,ProtLevel,BitRate]);