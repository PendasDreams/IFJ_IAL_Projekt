<?php
 declare(strict_types=1);

// Program 1: Vypocet faktorialu (iterativne)
/* 
ASDASD
AS
AS
*A*/

// Hlavni telo programu
//int write(int cock);
//write("Zadejte cislo pro vypocet faktorialu\n");
$a = 5;

if ($a === null) {
	write("Chyba pri nacitani celeho cisla \230 \n");
} else {}

if ($a < 0) {
	write("Faktorial nelze spocitat\n");
} else {
	$vysl = 1;
	while ($a > 0) {
		$vysl = $vysl * $a;
		$a = $a - 1;
	}
	write("Vysledek je: ", $vysl, "\n");
}
