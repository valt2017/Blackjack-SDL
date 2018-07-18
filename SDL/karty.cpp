#include "karty.h"
#include <algorithm>
#include <ctime>

using namespace oko;

karta::karta()
{
  indexKarty = 0;
  hodnotaKarty = 0;
  znakKarty = ' ';
}

karta::karta(unsigned int m_karta)
{
  indexKarty = m_karta;
  znakKarty = hodnoty[indexKarty % poc_hodnot];

  switch(znakKarty)
  {
    case 'A':
      hodnotaKarty = 11;
      break;

    case 'J':
    case 'Q':
    case 'K':
      hodnotaKarty = 10;
      break;

      default:
      hodnotaKarty = indexKarty % poc_hodnot + 1;
  }
}

unsigned int karta::vratHodnotu()
{
  return hodnotaKarty;
}

char karta::vratZnak()
{
  return znakKarty;
}

char karta::vratFarbu()
{
  return farby[indexKarty / poc_hodnot];
}

string karta::VypisKartu()
{
  stringstream str;
  str << vratFarbu() << vratZnak() << " ";
  return str.str();
}

balicek::balicek()
{
  srand(time(0));
  for(unsigned int i = 0; i < poc_kariet; i++)
    karty.push_back(karta(i));
  random_shuffle(karty.begin(), karty.end());
}

bool balicek::Prazdny()
{
  return(karty.empty());
}

karta balicek::DalsiaKarta()
{
  karta Kopia;
  if(!karty.empty())
  {
    Kopia = karty.back();
    karty.pop_back();
  }
  return(Kopia);
}

string balicek::VypisBalicek()
{
  stringstream str;

  str << "Karty v balicku : ";
  for(unsigned int i = 0; i < karty.size(); i++)
    str << karty[i].VypisKartu();
  str << "\n";
  return str.str();
}

hrac::hrac(string m_meno)
{
  skore = 0;
  meno = m_meno;
}

void hrac::PrijataKarta(karta nova)
{
  karty.push_back(nova);
  skore += nova.vratHodnotu();
}

string hrac::VypisHraca()
{
  stringstream str;
  str << meno << " skore: " << skore << " karty : ";
  for(unsigned int i = 0; i < karty.size(); i++)
    str << karty[i].VypisKartu();
  str << "\n";
  return str.str();
}

unsigned int hrac::VratSkore()
{
  return skore;
}

hrac hrac::operator < (hrac & h)
{ // iba porovnanie ziadny trop !
  if(skore > h.skore)
    return *this;
  return h;
}

karta hrac::operator[](unsigned int indexKarty) 
{
	return karty[indexKarty];
}

unsigned int hrac::PocetKariet()
{
	return karty.size();
}
