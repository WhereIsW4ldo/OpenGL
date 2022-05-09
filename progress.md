[x] raam: callback functie bij herschaling van het window; voorzie een optie in het programma
om te kunnen kiezen tussen een orthogonale, symmetrische perspectief of algemene perspectief transformatie; kies zelf gepaste waarden voor de verschillende argumenten van
deze functies;

[x] werf: basisfunctie waarin o.a. xyz-assen en plaats van lichtbronnen kunnen weergegeven
worden; deze functie roept de functies op om de verschillende elementen weer te geven;
met de toets n worden meerdere exemplaren van de kraan naast elkaar geplaatst;

[x] wand: een complex oppervlak gevormd door Bezier functies gebaseerd op 6 × 4 controlepunten (4 controlepunten in de breedte en 6 controlepunten in de lengte); dit oppervlak wordt gebruikt voor de voorwand van de cabine en een half tegengewicht; (voorzie eventueel toets l om de lijnenmesh te tonen);

[ ] last: (belangrijkste uitbreiding, ipv. bol) een complex oppervlak gevormd door B-spline
functies van orde 4 (graad 3) gebaseerd op C×C controlepunten met C >= 4; dit oppervlak
wordt gebruikt voor een achtste van de last; door dit oppervlak driemaal telkens over 90
graden te draaien komt een halve last tot stand; deze halve last kan dan nog gespiegeld
worden om een volledig volume te krijgen;

[x] licht: vier lichtbronnen die apart aan- en uit-geschakeld kunnen worden: ´e´en voor ambient
wit licht, ´e´en voor diffuus licht (vooral groen en blauw) en ´e´en voor specular licht (vooral
rood); kies zelf goede posities voor deze lichtbronnen zodat de effecten duidelijk kunnen
gedemonstreerd worden; een vierde lichtbron met geel ambient, geel diffuus, geel specular
licht en spoteffect wordt geplaatst op de cabine en gericht naar de last; deze lichtbron
draait mee met de kraanarm en cabine; in de basisopgave voorzie je voor de cabine, het vakwerk van toren en arm en de last
verschillende materiaaleigenschappen, bijv. grijs, brons en geel;

[ ] menu (uitbreiding): kiezen van de materiaaleigenschappen van cabine, vakwerk en last:

[x] toets: callback routine bij het indrukken van een toets. Voorzie x/X, y/Y , z/Z voor het
verplaatsen van het oog; a/A, b/B, c/C, d/D voor het aan/uitschakelen van de lichtbronnen; s/S voor de keuze tussen FLAT en SMOOTH shading;
met toets v/V kan eventueel de spothoek van de spot verkleind of vergroot worden; spot
exponent kan in stappen van 5 aangepast worden met toets w/W, initi¨ele waarde 20;
materiaaleigenschap shininess met initi¨ele waarde 10 kan in stappen van 5 aangepast
worden met toets e/E;
x met toets l kunnen de draadmodellen van de cylinder, schijven en complexe oppervlakken
al of niet getoond worden; 
x de xyz-assen kunnen met toets j al of niet getoond worden, en
de controlepunten van de oppervlakken met de toets k;
x meerdere exemplaren van de kraan tonen kan met de toets n;
hoogte van de spot aanpassen kan met de toetsen h/H;

[x] anim: de last kan horizontaal (toets g) en vertikaal (toets G) bewegen; het draaien van
de arm (inclusief bovenste schijf en cabine) kan met de toets r geactiveerd worden;

[ ] textuur (uitbreiding): de vlakke wanden van de cabine kunnen voorzien worden van een
textuur op basis van een kleurenpallet of een jpeg (in/uitschakelen met toets t); de last
en de kabel kunnen een eigen textuur krijgen; ook kan een grondoppervlak (bijvoorbeeld
kasseien) weergegeven worden;

[x] glas: het B´ezier oppervlak van de cabine is doorzichtig (toets f)
bij de materiaaleigenschappen van dit doorzichtig oppervlak een gepaste α nemen, zodat
o.a. de achter- en zijwanden en de bovenste platte schijf zichtbaar zijn;

[x] mist (uitbreiding): met fog kan ervoor gezorgd worden dat bij meerdere kranen de achterste kraan een waziger uitzicht krijgt (in/uitschakelen met toets m); met M kan eventueel
het exponenti¨ele effect geactiveerd worden.