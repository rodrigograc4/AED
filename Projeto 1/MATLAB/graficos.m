%% Solução 1 (PC Duarte)

valores = load("resultados_1_Duarte.txt");
pos_finais = valores(:,1);
tempos = valores(:,4);

figure(1);
plot(pos_finais,log10(tempos));
ylabel("Tempo (s)");
xlabel("Posição Final");
title("Solução fornecida");
hold on;

tempos_log = log10(tempos);
N = [pos_finais(20:end) 1+0*pos_finais(20:end)];
Coefs = pinv(N)*tempos_log(20:end);
Ntotal = [pos_finais pos_finais*0+1];

plot(pos_finais, Ntotal*Coefs, "k");
legend("Solução 1", "Regressão Linear");
grid on;
hold off;

t800_log = [800 1]*Coefs;
t800 = 10^t800_log;
fprintf('Tempo previsto até à posição 800 = %i\n',t800);

%% Solução 1 e Solução 2 (PC André)

valores = load("resultados_1_Andre.txt");
pos_finais1 = valores(:,1);
tempos1 = valores(:,4);

valores = load("resultados_2_Andre.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

figure(1);
semilogy(pos_finais1,tempos1, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 1 e 2")
hold on;
semilogy(pos_finais2,tempos2,"o-");
legend("Solução 1", "Solução 2");
grid on;
hold off;

%% Solução 1 e Solução 2 (PC duarte)

valores = load("resultados_1_Duarte.txt");
pos_finais1 = valores(:,1);
tempos1 = valores(:,4);

valores = load("resultados_2_Duarte.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

figure(1);
semilogy(pos_finais1,tempos1, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 1 e 2");
hold on;
semilogy(pos_finais2,tempos2,"o-");
legend("Solução 1", "Solução 2");
grid on;
hold off;

%% Solução 1 e Solução 2 (PC Rodrigo)

valores = load("resultados_1_Rodrigo.txt");
pos_finais1 = valores(:,1);
tempos1 = valores(:,4);

valores = load("resultados_2_Rodrigo.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

figure(1);
semilogy(pos_finais1,tempos1, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 1 e 2");
hold on;
semilogy(pos_finais2,tempos2,"o-");
legend("Solução 1", "Solução 2");
grid on;
hold off;

%% Solução 1 e Solução 2 (3 PC's)

valores = load("resultados_1_Andre.txt");
pos_finais1A = valores(:,1);
tempos1A = valores(:,4);

valores = load("resultados_2_Andre.txt");
pos_finais2A = valores(:,1);
tempos2A = valores(:,4);

valores = load("resultados_1_Duarte.txt");
pos_finais1D = valores(:,1);
tempos1D = valores(:,4);

valores = load("resultados_2_Duarte.txt");
pos_finais2D = valores(:,1);
tempos2D = valores(:,4);

valores = load("resultados_1_Rodrigo.txt");
pos_finais1R = valores(:,1);
tempos1R = valores(:,4);

valores = load("resultados_2_Rodrigo.txt");
pos_finais2R = valores(:,1);
tempos2R = valores(:,4);

figure(1);
semilogy(pos_finais1A,tempos1A, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 1 e 2");
hold on;
semilogy(pos_finais2A,tempos2A,"o-");
semilogy(pos_finais1D,tempos1D,"+-");
semilogy(pos_finais2D,tempos2D,"o-");
semilogy(pos_finais1R,tempos1R,"+-");
semilogy(pos_finais2R,tempos2R,"o-");
legend("Solução 1 - PC Andre", "Solução 2 - PC André", ...
        "Solução 1 - PC Duarte", "Solução 2 - PC Duarte", ...
        "Solução 1 - PC Rodrigo", "Solução 2 - PC Rodrigo");
grid on;
hold off;

%% Solução 2 e Solução 3 (PC André)

valores = load("resultados_2_Andre.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

valores = load("resultados_3_Andre.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

figure(1);
semilogy(pos_finais2,tempos2, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 2 e 3");
hold on;
semilogy(pos_finais3,tempos3,"o-");
legend("Solução 2", "Solução 3");
grid on;
hold off;

%% Solução 2 e Solução 3 (PC Duarte)

valores = load("resultados_2_Duarte.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

valores = load("resultados_3_Duarte.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

figure(1);
semilogy(pos_finais2,tempos2, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 2 e 3");
hold on;
semilogy(pos_finais3,tempos3,"o-");
legend("Solução 2", "Solução 3");
grid on;
hold off;

%% Solução 2 e Solução 3 (PC Rodrigo)

valores = load("resultados_2_Rodrigo.txt");
pos_finais2 = valores(:,1);
tempos2 = valores(:,4);

valores = load("resultados_3_Rodrigo.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

figure(1);
semilogy(pos_finais2,tempos2, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 2 e 3");
hold on;
semilogy(pos_finais3,tempos3,"o-");
legend("Solução 2", "Solução 3");
grid on;
hold off;

%% Solução 2 e Solução 3 (3 PC's)

valores = load("resultados_2_Andre.txt");
pos_finais2A = valores(:,1);
tempos2A = valores(:,4);

valores = load("resultados_3_Andre.txt");
pos_finais3A = valores(:,1);
tempos3A = valores(:,4);

valores = load("resultados_2_Duarte.txt");
pos_finais2D = valores(:,1);
tempos2D = valores(:,4);

valores = load("resultados_3_Duarte.txt");
pos_finais3D = valores(:,1);
tempos3D = valores(:,4);

valores = load("resultados_2_Rodrigo.txt");
pos_finais2R = valores(:,1);
tempos2R = valores(:,4);

valores = load("resultados_3_Rodrigo.txt");
pos_finais3R = valores(:,1);
tempos3R = valores(:,4);

figure(1);
semilogy(pos_finais2A,tempos2A, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 2 e 3");
hold on;
semilogy(pos_finais3A,tempos3A,"+-");
semilogy(pos_finais2D,tempos2D,"o-");
semilogy(pos_finais3D,tempos3D,"o-");
semilogy(pos_finais2R,tempos2R,".-");
semilogy(pos_finais3R,tempos3R,".-");
legend("Solução 2 - PC Andre", "Solução 3 - PC André", ...
        "Solução 2 - PC Duarte", "Solução 3 - PC Duarte", ...
        "Solução 2 - PC Rodrigo", "Solução 3 - PC Rodrigo");
grid on;
hold off;

%% Solução 3 e Solução 4 (PC André)

valores = load("resultados_3_Andre.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_4_Andre.txt");
pos_finais4 = valores(:,1);
tempos4 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 4");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais4,tempos4,"o-");
legend("Solução 3", "Solução 4");
grid on;
hold off;

%% Solução 3 e Solução 4 (PC Duarte)

valores = load("resultados_3_Duarte.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_4_Duarte.txt");
pos_finais4 = valores(:,1);
tempos4 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 4");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais4,tempos4,"o-");
legend("Solução 3", "Solução 4");
grid on;
hold off;

%% Solução 3 e Solução 4 (PC Rodrigo)

valores = load("resultados_3_Rodrigo.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_4_Rodrigo.txt");
pos_finais4 = valores(:,1);
tempos4 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 4");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais4,tempos4,"o-");
legend("Solução 3", "Solução 4");
grid on;
hold off;

%% Solução 3 e Solução 4 (3 PC's)

valores = load("resultados_3_Andre.txt");
pos_finais3A = valores(:,1);
tempos3A = valores(:,4);

valores = load("resultados_4_Andre.txt");
pos_finais4A = valores(:,1);
tempos4A = valores(:,4);

valores = load("resultados_3_Duarte.txt");
pos_finais3D = valores(:,1);
tempos3D = valores(:,4);

valores = load("resultados_4_Duarte.txt");
pos_finais4D = valores(:,1);
tempos4D = valores(:,4);

valores = load("resultados_3_Rodrigo.txt");
pos_finais3R = valores(:,1);
tempos3R = valores(:,4);

valores = load("resultados_4_Rodrigo.txt");
pos_finais4R = valores(:,1);
tempos4R = valores(:,4);

figure(1);
semilogy(pos_finais3A,tempos3A, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 4");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais4A,tempos4A,"+-");
semilogy(pos_finais3D,tempos3D,"o-");
semilogy(pos_finais4D,tempos4D,"o-");
semilogy(pos_finais3R,tempos3R,".-");
semilogy(pos_finais4R,tempos4R,".-");
legend("Solução 3 - PC Andre", "Solução 4 - PC André", ...
        "Solução 3 - PC Duarte", "Solução 4 - PC Duarte", ...
        "Solução 3 - PC Rodrigo", "Solução 4 - PC Rodrigo");
grid on;
hold off;

%% Solução 3 e Solução 5 (PC André)

valores = load("resultados_3_Andre.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_5_Andre.txt");
pos_finais5 = valores(:,1);
tempos5 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 5");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais5,tempos5,"o-");
legend("Solução 3", "Solução 5");
grid on;
hold off;

%% Solução 3 e Solução 5 (PC Duarte)

valores = load("resultados_3_Duarte.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_5_Duarte.txt");
pos_finais5 = valores(:,1);
tempos5 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 5");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais5,tempos5,"o-");
legend("Solução 3", "Solução 5");
grid on;
hold off;

%% Solução 3 e Solução 5 (PC Rodrigo)

valores = load("resultados_3_Rodrigo.txt");
pos_finais3 = valores(:,1);
tempos3 = valores(:,4);

valores = load("resultados_5_Rodrigo.txt");
pos_finais5 = valores(:,1);
tempos5 = valores(:,4);

figure(1);
semilogy(pos_finais3,tempos3, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 5");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais5,tempos5,"o-");
legend("Solução 3", "Solução 5");
grid on;
hold off;

%% Solução 3 e Solução 5 (3 PC's)

valores = load("resultados_3_Andre.txt");
pos_finais3A = valores(:,1);
tempos3A = valores(:,4);

valores = load("resultados_5_Andre.txt");
pos_finais5A = valores(:,1);
tempos5A = valores(:,4);

valores = load("resultados_3_Duarte.txt");
pos_finais3D = valores(:,1);
tempos3D = valores(:,4);

valores = load("resultados_5_Duarte.txt");
pos_finais5D = valores(:,1);
tempos5D = valores(:,4);

valores = load("resultados_3_Rodrigo.txt");
pos_finais3R = valores(:,1);
tempos3R = valores(:,4);

valores = load("resultados_5_Rodrigo.txt");
pos_finais5R = valores(:,1);
tempos5R = valores(:,4);

figure(1);
semilogy(pos_finais3A,tempos3A, "+-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação das soluções 3 e 5");
ylim([0 1e-3]);
hold on;
semilogy(pos_finais5A,tempos5A,"+-");
semilogy(pos_finais3D,tempos3D,"o-");
semilogy(pos_finais5D,tempos5D,"o-");
semilogy(pos_finais3R,tempos3R,".-");
semilogy(pos_finais5R,tempos5R,".-");
legend("Solução 3 - PC Andre", "Solução 5 - PC André", ...
        "Solução 3 - PC Duarte", "Solução 5 - PC Duarte", ...
        "Solução 3 - PC Rodrigo", "Solução 5 - PC Rodrigo");
grid on;
hold off;

%% 5 Soluções (3 PC's)

valores = load("resultados_1_Andre.txt");
pos_finais1A = valores(:,1);
tempos1A = valores(:,4);

valores = load("resultados_2_Andre.txt");
pos_finais2A = valores(:,1);
tempos2A = valores(:,4);

valores = load("resultados_3_Andre.txt");
pos_finais3A = valores(:,1);
tempos3A = valores(:,4);

valores = load("resultados_4_Andre.txt");
pos_finais4A = valores(:,1);
tempos4A = valores(:,4);

valores = load("resultados_5_Andre.txt");
pos_finais5A = valores(:,1);
tempos5A = valores(:,4);

valores = load("resultados_1_Duarte.txt");
pos_finais1D = valores(:,1);
tempos1D = valores(:,4);

valores = load("resultados_2_Duarte.txt");
pos_finais2D = valores(:,1);
tempos2D = valores(:,4);

valores = load("resultados_3_Duarte.txt");
pos_finais3D = valores(:,1);
tempos3D = valores(:,4);

valores = load("resultados_4_Duarte.txt");
pos_finais4D = valores(:,1);
tempos4D = valores(:,4);

valores = load("resultados_5_Duarte.txt");
pos_finais5D = valores(:,1);
tempos5D = valores(:,4);

valores = load("resultados_1_Rodrigo.txt");
pos_finais1R = valores(:,1);
tempos1R = valores(:,4);

valores = load("resultados_2_Rodrigo.txt");
pos_finais2R = valores(:,1);
tempos2R = valores(:,4);

valores = load("resultados_3_Rodrigo.txt");
pos_finais3R = valores(:,1);
tempos3R = valores(:,4);

valores = load("resultados_4_Rodrigo.txt");
pos_finais4R = valores(:,1);
tempos4R = valores(:,4);

valores = load("resultados_5_Rodrigo.txt");
pos_finais5R = valores(:,1);
tempos5R = valores(:,4);

figure(1);
semilogy(pos_finais1A,tempos1A, "o-");
ylabel("log(Tempo (s))");
xlabel("Posição Final");
title("Comparação de todas as soluções nos 3 PC's");
hold on;
semilogy(pos_finais2A,tempos2A,"o-");
semilogy(pos_finais3A,tempos3A,"o-");
semilogy(pos_finais4A,tempos4A,"o-");
semilogy(pos_finais5A,tempos5A,"o-");
semilogy(pos_finais1D,tempos1D,"+-");
semilogy(pos_finais2D,tempos2D,"+-");
semilogy(pos_finais3D,tempos3D,"+-");
semilogy(pos_finais4D,tempos4D,"+-");
semilogy(pos_finais5D,tempos5D,"+-");
semilogy(pos_finais1R,tempos1R,".-");
semilogy(pos_finais2R,tempos2R,".-");
semilogy(pos_finais3R,tempos3R,".-");
semilogy(pos_finais4R,tempos4R,".-");
semilogy(pos_finais5R,tempos5R,".-");
legend("Solução 1 - PC Andre", "Solução 2 - PC André", ...
        "Solução 3 - PC André", "Solução 4 - PC André", ...
        "Solução 5 - PC André", "Solução 1 - PC Duarte", ...
        "Solução 2 - PC Duarte", "Solução 3 - PC Duarte", ...
        "Solução 4 - PC Duarte", "Solução 5 - PC Duarte",...
        "Solução 1 - PC Rodrigo", "Solução 2 - PC Rodrigo", ...
        "Solução 3 - PC Rodrigo", "Solução 4 - PC Rodrigo", ...
        "Solução 5 - PC Rodrigo");
grid on;
hold off;