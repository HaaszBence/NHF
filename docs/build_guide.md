# Projekt Architektúra és Fejlesztői Útmutató
**C + Raylib Inkremens (Klikker) Játék**

Ez az útmutató összefoglalja a projekt felépítését, az egyes rétegek és fájlok pontos felelősségét, valamint a biztonságos fejlesztési sorrendet.

---

## 1. Mappaszerkezet és Hordozhatóság

A projekt felépítése úgy lett kialakítva, hogy otthon (Windows + `C:\raylib`) és a laboros (HSZK) gépeken is egyaránt, külön konfigurálás nélkül leforduljon.

```text
projekt_gyoker/
├── assets/          # Statikus erőforrások (betűtípusok, hangok – csak olvassuk)
├── build/           # A lefordított futtatható fájl (.exe)
├── data/            # Dinamikus futásidejű adatok (save.txt, settings.txt – ide írunk)
├── src/             # Saját C forráskódok és fejlécek
│   ├── main.c       # A program belépési pontja ("karmester")
│   ├── core/        # Alapvető típusok, konstansok, média betöltés
│   ├── logic/       # Játéklogika, matematika, állapotfrissítés, mentés
│   └── view/        # Grafikus megjelenítés (csak olvasás, rajzolás)
└── vendor/          # Külső könyvtárak (hordozhatóság miatt a projektben tárolva)
    ├── include/     # raylib.h
    └── lib/         # libraylib.a
```

---

## 2. A Három Réteg Szabályai

A tiszta kód és a könnyű hibakeresés érdekében három szigorúan elválasztott rétegben dolgozunk:

1. **CORE (Alapok):** Közös típusok és nyers erőforrások. Nem ismeri a játékmenetet, a boltot vagy a pontszámokat.
2. **LOGIC (Agymunka):** A játék matematikája és szabályai. **Vak réteg:** nem hívhat meg egyetlen Raylib rajzoló (`Draw...`) függvényt sem. Kizárólag adatokat számol és módosít.
3. **VIEW (Megjelenítés):** A grafikus felület. **Csak olvasható réteg:** az adatokat `const` mutatókon keresztül kapja meg. Semmit nem számol ki a játékállásból, kizárólag kirajzolja az aktuális állapotot.

---

## 3. Fájlok Pontos Szerepe

| Réteg / Fájl | Fő feladat | Mit tartalmaz konkrétan? |
| :--- | :--- | :--- |
| **`src/main.c`** | **A karmester** | Ablak és hangrendszer indítása, a rétegek összekötése, a főciklus (`while (!WindowShouldClose())`) futtatása, kilépéskor minden memória felszabadítása. |
| **CORE** | | |
| `core/state.h` | Közös szótár | Az alkalmazás főállapotai (`STATE_MENU`, `STATE_GAME`, `STATE_SETTINGS`, `STATE_EXIT`), a játékon belüli fülek (`VIEW_CLICK`, `VIEW_SHOP`, `VIEW_TREE`) és ablakkonstansok. |
| `core/assets.h/.c` | Média kezelés | Betűtípusok (`LoadFontEx` magyar ékezetekkel és matek szimbólumokkal) és hanghatások (`LoadSound`) betöltése, majd felszabadítása. |
| `core/utils.h/.c` | Segédfüggvények | Általános, bárhol használható segédeszközök (pl. nagy számok formázása: $1\,500\,000 \rightarrow$ `"1.50 M"` vagy `"1.50e6"`). |
| `core/debugmalloc.h` | Memória ellenőrzés | Memóriaszivárgások detektálása a dinamikusan foglalt tömbökhöz. |
| **LOGIC** | | |
| `logic/game.h/.c` | Fő játékállapot | A központi `GameData` struktúra (aktuális pontszám, kattintási erő, másodpercenkénti termelés), valamint a fő frissítő függvény (`dt` alapú termelés, kattintás kezelése). |
| `logic/shop.h/.c` | Bolt logika | A megvásárolható termelők listája, árak exponenciális növekedése, vásárlási feltétel ellenőrzése és pontlevonás. |
| `logic/tree.h/.c` | Fejlesztési fa | A matekos upgrade-ek (lemmák, tételek) csomópontjai, előfeltételek ellenőrzése és szorzók kiszámítása. |
| `logic/storage.h/.c` | Mentés / Betöltés | A `GameData` és a beállítások kiírása a `data/save.txt` és `data/settings.txt` fájlokba, illetve azok visszaolvasása induláskor. |
| **VIEW** | | |
| `view/render.h/.c` | Fő megjelenítő | A `main.c` egyetlen rajzolási belépési pontja. Eldönti az állapot alapján, mit rajzoljon ki (menü, beállítások vagy maga a játék), és kirajzolja a közös felső állapotsávot. |
| `view/view_*.h/.c` | Alnézetek *(opcionális)* | Ha a `render.c` túl hosszúra nő, ide szervezhetők ki a külön fülek rajzolásai (`view_shop.c`, `view_tree.c`). |

---

## 4. Hogyan építsd fel lépésről lépésre?

Ne egyszerre írd meg az összes fájlt! Haladj működő mérföldkövekben:

### 1. mérföldkő: A minimális mag (Kattintható pontszám)
* **`core/state.h`**: Hozd létre a `MainState` és `GameView` enumokat.
* **`core/assets.h/.c`**: Tölts be egyetlen betűtípust (`DejaVuSans.ttf`).
* **`logic/game.h/.c`**: Hozz létre egy egyszerű `GameData` struktúrát csak egy `double points;` mezővel, és egy függvényt, ami kattintásra növeli ezt az értéket.
* **`view/render.h/.c`**: Írd ki a képernyő közepére az aktuális `points` értékét.
* **`src/main.c`**: Kösd össze őket. *(Eredmény: egy működő, kattintható ablak, ahol nő a szám.)*

### 2. mérföldkő: Állapotgép és Menü
* Egészítsd ki a `main.c`-t és a `render.c`-t úgy, hogy a játék `STATE_MENU` állapotból induljon, és egy „Start” gombbal lehessen belépni a `STATE_GAME` nézetbe.

### 3. mérföldkő: Bolt és Passzív Termelés
* **`logic/shop.h/.c`**: Definiáld a bolt elemeit (név, alapár, mennyiség, termelés/mp).
* **`logic/game.c`**: A `dt` (eltelt idő) segítségével add hozzá a passzív termelést minden képkockában (`points += cps * dt`).
* **`view/render.c`**: Készítsd el a fülváltást (`VIEW_CLICK` és `VIEW_SHOP`), és rajzold ki a megvásárolható elemeket.

### 4. mérföldkő: Fejlesztési Fa és Mentés
* **`logic/tree.h/.c`**: Add hozzá a speciális szorzókat adó fejlesztéseket.
* **`logic/storage.h/.c`**: Implementáld a fájlba mentést (`data/save.txt`) és a beállítások mentését (`data/settings.txt`), majd ellenőrizd a memóriakezelést a `debugmalloc` segítségével.

---

## 5. Gyors Ellenőrzőlista Hibakereséshez

* **Elcsúszott egy gomb vagy rossz a színe?** $\rightarrow$ Nézd meg a `src/view/` mappát.
* **Rosszul számolja az árat vagy a pontokat?** $\rightarrow$ Nézd meg a `src/logic/` mappát.
* **Kérdőjelek jelennek meg az ékezetes betűk helyén?** $\rightarrow$ Ellenőrizd a `LoadFontEx` kódpontjait a `src/core/assets.c`-ben.
* **Nem menti el az állást kilépéskor?** $\rightarrow$ Ellenőrizd, hogy létezik-e a `data/` mappa a projekt gyökerében, és meghívódik-e a `storage_save_game()` a `main.c` végén.