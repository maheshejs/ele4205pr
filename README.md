# Projet final ELE4205

## Contexte

Cette application client-serveur permet de capturer des images côté serveur et les envoyer au client via le protocole TCP/IP. Le client affiche ces images et peut demander quatre résolutions différentes à l'aide des touches 1 à 4 du clavier.

Les images envoyées au client sont censées contenir du texte représentant de la musique sous forme codifiée. Lorsque l'on appuie sur un bouton côté serveur, le client extrait le texte signifiant, corrige si nécessaire quelques erreurs et renvoie le texte au serveur pour celui-ci fasse jouer la musique.

## Directives de compilation

Pour la compilation, exécuter ces commandes dans le dossier où se trouve le projet cloné.

```
bash
mkdir -p build/native build/odroid
cd build/native
cmake -DCMAKE_BUILD_TYPE=Release ../../
make mainClient
cd ../odroid
source <path>/opt/poky/environment-setup-aarch64-poky-linux # Remplacer <path> par le chemin vers le dossier /opt/ contenant le SDK
cmake -DCMAKE_BUILD_TYPE=Release ../../
make mainImageServer mainMusicServer
```

## Exécution des programmes client et serveur

Il faut ensuite copier les fichiers binaires sur le odroid par `scp` en s'assurant d'avoir configuré au préalable la connexion TCP/IP entre l'odroid et l'ordinateur.

```
scp bin/release/mainImageServer bin/release/mainMusicServer root@192.168.7.2:~/
```

On démarre le serveur sur l'odroid par les commandes suivantes.

```
sudo /users/Cours/ele4205/commun/scripts/ifconfig-enp0s-up
ssh root@192.168.7.2
echo 228 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio228/direction
cd /sys/devices/
modprobe pwm-meson
modprobe pwm-ctrl
cd pwm-ctrl.42/
echo 512 > duty0
cd ~
./mainImageServer &
bg
exit
```

On démarre le client sur l'ordinateur par les commandes suivantes.

```
cd ../native
./bin/release/mainClient 192.168.7.2
```

## Usage

Sur l'ordinateur, on peut changer la résolution de l'image à l'aide des touches 1 à 4 du clavier.

Sur l'odroid, on déclenche la série d'événements menant à faire jouer la musique en appuyant sur le bouton.
