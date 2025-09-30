# ASCIIVideoPlayer
**ASCIIVideoPlayer** vuole essere un semplice riproduttore di video già convertiti in formato ASCII per terminale.

## Funzionamento

**ASCIIVideoPlayer** non richiede parametri in linea di comando per iniziare la sua esecuzione. Tuttavia, necessita di alcune informazioni.

Il file `config`, posto nella stessa directory dell'eseguibile, viene letto da **ASCIIVideoPlayer** prima di cominciare la riproduzione, proprio per acquisire i parametri necessari al suo funzionamento:

- Il percorso della directory in cui sono presenti i frames già convertiti in formato *ASCII*;
- Il percorso del file per la riproduzione dell'audio;
- Il numero di frames al secondo che saranno riprodotti dalla finestra.

**ASCIIVideoPlayer** caricherà i frames in memoria e comincerà la riproduzione di essi e dell'audio relativo.

Il programma terminerà in autonomia quando sia la traccia audio sia i frames in riproduzione saranno esauriti.

### Conversione fotogrammi

**ASCIIVideoPlayer** non provvede direttamente a convertire un video in fotogrammi e i fotogrammi in formato *ASCII*. È necessario utilizzare degli strumenti esterni per eseguire le conversioni.

Se si utilizza un sistema con Windows si consiglia di utilizzare come dimensioni dei fotogrammi $120*30$, per rispettare la dimensione predefinita del terminale.

### Librerie esterne

**ASCIIVideoPlayer** utilizza le librerie `SDL2` e `SDL2_mixer` per permettere la riproduzione dell'audio della finestra.

## Compilazione

La compilazione dell'eseguibile avviene in maniera automatizzata tramite `Makefile`. 

Il file non è stato generato da `CMake`. È opportuno fare quindi attenzione a sue eventuali modifiche.

### Linux

La compilazione è automatica tramite `make`.

È necessario scaricare le librerie `SDL2` e `SDL2_mixer`. 
- Se l'installazione avviene tramite utente `sudo` e globalmente nel sistema, la compilazione dovrebbe essere eseguita senza problemi;
- Se l'installazione avviene in maniera locale o comunque differente da quella sopra citata, sarà necessario modificare il file `Makefile` per indicare la posizione delle librerie.

### Windows

La compilazione è automatica tramite `Make`.

È necessario scaricare le librerie `SDL2` e `SDL2_mixer`.

La posizione predefinita per le librerie è la directory del progetto. Eventuali modifiche comportano un conseguente adeguamento del file di compilazione.

Prima di lanciare l'eseguibile, è necessario copiare le relative `dll` nella medesima directory dello stesso.
