clear
clc
load("data.mat")

% dt = nsr001; %input
dt = chf206; %input
xmean = mean(dt);
y = dt - xmean;
yk = cumsum(y);

n = 4:64;
s = length(n);

f_j = zeros(61,1);

for i=1:s
% for i=1:1
    j = n(i);
    m = mod(length(yk),j);
    len = (length(yk)-m)/j;
    p_add = [];
    if m==0
        Y = reshape(yk,j,[]);
        T = reshape(dt,j,[]);
    elseif (m>0) && (m<=2)
        Y = reshape(yk(1:len*j),j,[]);
        T = reshape(dt(1:len*j),j,[]);
%         mess = ['m: ', num2str(m), ' j: ', num2str(n(i))];
%         disp(mess)
    else
        Y = reshape(yk(1:len*j),j,[]);
        T = reshape(dt(1:len*j),j,[]);
        Y_ADD = yk(end-m+1:end);
        T_ADD = dt(end-m+1:end);
        p_add = polyfit(T_ADD',Y_ADD',1);
    end
    p = zeros(size(T,1),2);
    yf = zeros(size(Y));
    sz = size(T,1);
    for k=1:sz
        p(k,:) = polyfit(T(k,:),Y(k,:),1);
        yf(k,:) = polyval( p(k,:), T(k,:) );
    end
    diff = Y-yf;
    diff2 = diff.^2;
    l = length(diff2);
    summ = sum(diff2, 'all');
    f_j(i) = sqrt( (1/l) * summ.^2  );
end


figure
plot( n, f_j)
title("f_j", 'Interpreter','none')

figure
loglog( n, f_j)
title("loglog f_j", 'Interpreter','none')

f_log = log10(f_j);
n_log = log10(n);

figure
plot(n_log, f_log)
title("f_log", 'Interpreter','none')

n_a1 = n_log(1:16);
f_a1 = f_log(1:16);

n_a2 = n_log(16:end);
f_a2 = f_log(16:end);

pf1 = polyfit(n_a1,f_a1,1);
alfa1 = pf1(1)

pf2 = polyfit(n_a2,f_a2,1);
alfa2 = pf2(1)

% if a=0.5, there is no correlation and the signal is an 
% uncorrelated signal (white noise); 
% if a<0.5, the signal is anticorrelated;
% if a > 0.5, there are positive correlations in the signal.

% alfa1 = 0.5 - sygnał nieskorelowany
% alfa2 ~ 0.5 - sygnał nieskorelowany

% f_alfa1= polyval( pf1, n_log(1:16) );


% DONE:
% todo1 - polyval odcinków z tablicy p
% todo2 - wyliczenie średniej kwadratowej fluktuacji dla
%       każdej długości przedziału
% todo3 - wyznaczenie polyfit i polyval dla macierzy z todo2
% todo4 - znalezienie zależności F(n) - n^a i wyznaczenie
%       wartości a i b, gdzie a będzie dla zależności, gdzie
%       długość przedziału wynosi od 4 do 16, a b dla
%       przedziałów o długości 16 do N (tutaj 64)
