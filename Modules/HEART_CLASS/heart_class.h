#ifndef DADM_HEART_CLASS_H
#define DADM_HEART_CLASS_H

#include <iostream>
#include <valarray>
#include <vector>

/* Mozliwe typy aktywacji */
enum typeOfActivation{
    DIFF_DISEASE,
    VENTRICULAR,
    SUPRAVENTRICULAR,
    ARTIFACT,
};

/* Struktura zawierajaca index i typ klasyfikacji -> STRUKTURA WYJSCIOWA */
typedef struct activations{
    float Ridx;             /* index R piku */
    enum typeOfActivation actType;      /* typ aktywacji (enum type) */
}activations_t;

/* Glowna klasa modulu
 * Po wywolaniu konstruktora otrzymujemy wskaznik do wektora struktur aktywacji
 */
class HeartActivationClassifier {
private:
/*
 * Funkcja sprawdzajaca obecnosc dysocjacji AV.
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       currR - aktualny pik R
 * OUT:      rodzaj aktywacji.
 */
    enum typeOfActivation CheckAVDissociation(std::vector<int> P, int currR, std::vector<int> QRSonset1);

/*
 * Funkcja do klasyfikacji
 * IN:       rPeaks - wektor pozycji zalamkow R
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       fs - czestotliwosc probkowania
 */
    void ClassifyType(std::vector<int> rPeaks, std::vector<int> P, std::vector<int> QRSend, std::vector<int> QRSonset, int fs);

    std::vector<activations_t> activations;
    std::vector <float> m_filteredSignal;
    std::vector <int> m_rPeaks;
    std::vector<int> m_rPeaksLocs;
    std::vector <int> m_QRSend;
    std::vector <int> m_QRSonset;
    std::vector <int> m_P;
public:

/* Konstruktor
 * IN:       rPeaks - wektor pozycji zalamkow R
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       fs - czestotliwosc probkowania
 */
    HeartActivationClassifier(std::vector<int> rPeaks, std::vector<int> P, std::vector<int> QRSend, std::vector<int> QRSonset)
    {
            m_rPeaks = rPeaks;
            m_QRSend = QRSend;
            m_QRSonset = QRSonset;
            m_P = P;
            ClassifyType(rPeaks, P, QRSend, QRSonset, 360);
    }

/* Funkcja pobierajaca kolejne aktywacje
 * OUT:      wskaznik do wektora aktywacji.
 */
    std::shared_ptr<std::vector<activations_t>> HeartActivationClassifierGetActivations()
    {
        return std::make_shared<std::vector<activations_t>>(this->activations);
    }
};
#endif //DADM_HEART_CLASS_H