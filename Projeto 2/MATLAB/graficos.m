%% Hash Table Size Big

valores = load("hashTableSizeBig.txt");
hashTableSize = valores(:,1);
numEntries = valores(:,2);

plot(numEntries,hashTableSize);
axis([0,1000000,0,1200000]);
ylabel("Hash Table Size");
xlabel("Number of Entries");

%% Gráfico Barras Big

valores = load("histogramaBig.txt");
indexs = valores(:,1);
numWords = valores(:,2);

bar(indexs,numWords);
ylabel("Number of Words");
xlabel("Index");

%% Grafo Big

data = readtable("graphBig.txt");
G = graph;
words = unique(data(:,1));
for i= 1:height(words)
    G = addnode(G,words{i,1});
end
for i= 1:height(data)
    G = addedge(G,data{i,1},data{i,2});
end

plot(G)

%% Hash Table Size Four

valores = load("hashTableSizeFour.txt");
hashTableSize = valores(:,1);
numEntries = valores(:,2);

plot(numEntries,hashTableSize);
axis([0,2500,0,4100]);
ylabel("Hash Table Size");
xlabel("Number of Entries");

%% Gráfico Barras Four

valores = load("histogramaFour.txt");
indexs = valores(:,1);
numWords = valores(:,2);

bar(indexs,numWords);
ylabel("Number of Words");
xlabel("Index");

%% Grafo Four

data = readtable("graphFour.txt");
G = graph;
words = unique(data(:,1));
for i= 1:height(words)
    G = addnode(G,words{i,1});
end
for i= 1:height(data)
    G = addedge(G,data{i,1},data{i,2});
end

plot(G)

%% Hash Table Size Five

valores = load("hashTableSizeFive.txt");
hashTableSize = valores(:,1);
numEntries = valores(:,2);

plot(numEntries,hashTableSize);
axis([0,8000,0,8100]);
ylabel("Hash Table Size");
xlabel("Number of Entries");

%% Gráfico Barras Five

valores = load("histogramaFive.txt");
indexs = valores(:,1);
numWords = valores(:,2);

bar(indexs,numWords);
ylabel("Number of Words");
xlabel("Index");

%% Grafo Five

data = readtable("graphFive.txt");
G = graph;
words = unique(data(:,1));
for i= 1:height(words)
    G = addnode(G,words{i,1});
end
for i= 1:height(data)
    G = addedge(G,data{i,1},data{i,2});
end

plot(G)

%% Hash Table Size Six

valores = load("hashTableSizeSix.txt");
hashTableSize = valores(:,1);
numEntries = valores(:,2);

plot(numEntries,hashTableSize);
axis([0,16000,0,16100]);
ylabel("Hash Table Size");
xlabel("Number of Entries");

%% Gráfico Barras Six

valores = load("histogramaSix.txt");
indexs = valores(:,1);
numWords = valores(:,2);

bar(indexs,numWords);
ylabel("Number of Words");
xlabel("Index");

%% Grafo Six

data = readtable("graphSix.txt");
G = graph;
words = unique(data(:,1));
for i= 1:height(words)
    G = addnode(G,words{i,1});
end
for i= 1:height(data)
    G = addedge(G,data{i,1},data{i,2});
end

plot(G)