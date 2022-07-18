#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <chrono>
#include <thread>

// Method declarations
void shuffle();
void display(int index, int delay);
void swap(int* a, int* b);
void selectionSort();
void insertionSort();
void merge(int p, int q, int r);
int  partition(int p, int q);
void mergeSort(int start, int end);
void quickSort(int p, int q);
void heapify(int n, int i);
void heapSort(int n);
void bubbleSort();

// Global Variables
sf::RenderWindow viewport(sf::VideoMode(1200, 600), "Sorting Algorithm Visualizer");
sf::SoundBuffer buffer;
sf::Sound beep;
int rects[100];
bool sorted;

int main()
{
    // If the sound cannot be found in the files
    // The Viewport closes and the program ends
    if (!buffer.loadFromFile("beep.wav")) {
        viewport.close();
        return 0;
    }
    else // If the sound is found
    {
        beep.setBuffer(buffer);
        beep.play();
    }

    // Seeding random to get different numbers each time
    srand(time(NULL));

    shuffle();

    while (viewport.isOpen())
    {
        sf::Event e;
        while (viewport.pollEvent(e))
        {
            // Closes the window and ends the program
            if (e.type == sf::Event::Closed)
            {
                viewport.close();
                return 0;
            }

            // Runs insertion sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                shuffle();
                insertionSort();
                display(0, 10);
                sorted = true;
            }

            // Runs merge sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                shuffle();
                mergeSort(0, 99);
                display(0, 10);
                sorted = true;
            }

            // Runs quick sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                shuffle();
                quickSort(0, 99);
                display(0, 10);
                sorted = true;
            }

            // Runs heap sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                shuffle();
                heapSort(100);
                display(0, 10);
                sorted = true;
            }

            // Runs selection sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
                shuffle();
                selectionSort();
                display(0, 10);
                sorted = true;
            }

            // Runs bubble sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
                shuffle();
                bubbleSort();
                display(0, 1);
                sorted = true;
            }

            // Runs insertion sort
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) shuffle();

            // Displays current sorted array for debugging
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) display(0, 1);
        }
    }

    delete[] rects;
}

// Shuffles the number bars
void shuffle()
{
    sorted = false;
    for (int i = 0; i < 100; i++)
    {
        rects[i] = (rand() % 500);
    }

    display(0, 10);
}

// Displays the number bars and changes the colors
void display(int index, int delay)
{
    viewport.clear();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    for (int i = 0; i < 100; i++) {
        sf::RectangleShape bar(sf::Vector2f(10, rects[i]));
        bar.setPosition(i * 12, 600 - rects[i]);
        bar.setFillColor(sorted || i == index ? sf::Color::Green : sf::Color::White);
        viewport.draw(bar);
    }

    viewport.display();
}

// Plays the sound that correlates to the bar being interacted with
void playSound(int index)
{
    // Changes the sound's pitch to match the size of the rectangle
    beep.setPitch(.50f + (float)index / 100.0f);
    beep.play();
}

// swaps two elements in the array
void swap(int* a, int* b)
{
    auto temp = *a;
    *a = *b;
    *b = temp;
}

// Best: O(n^2), Worst: O(n^2), Average: O(n^2)
void selectionSort() {
    int minIndex = 0;

    for (int i = 0; i < 99; i++)
    {
        minIndex = i;

        for (int j = i + 1; j < 100; j++)
        {
            if (rects[j] < rects[minIndex])
            {
                minIndex = j;
                display(j, 10);
                playSound(j);
            }
        }

        display(i, 10);
        playSound(i);
        swap(&rects[minIndex], &rects[i]);
    }
}

// Best: O(n), Worst: O(n^2), Average: O(n^2)
void insertionSort()
{
    int key, i, j;

    for (i = 0; i < 100; i++)
    {
        key = rects[i];
        j = i - 1;

        while (j >= 0 && rects[j] > key)
        {
            rects[j + 1] = rects[j];
            j = j - 1;
            display(j, 10);
            playSound(j);
        }

        rects[j + 1] = key;
    }
}

// Best: O(nlogn), Worst: O(nlogn), Average: O(nlogn)
void merge(int p, int q, int r)
{
    int const leftNum = q - p + 1;
    int const rightNum = r - q;

    int* leftArr = new int[leftNum];
    int* rightArr = new int[rightNum];

    for (int i = 0; i < leftNum; i++) leftArr[i] = rects[p + i];
    for (int j = 0; j < rightNum; j++) rightArr[j] = rects[q + j + 1];

    int leftIndex = 0, rightIndex = 0, mergeIndex = p;

    // Merging the temp arrays into the rects array
    while (leftIndex < leftNum && rightIndex < rightNum)
    {
        if (leftArr[leftIndex] <= rightArr[rightIndex])
        {
            rects[mergeIndex] = leftArr[leftIndex++];
            display(mergeIndex, 10);
            playSound(mergeIndex);
            mergeIndex++;
        }
        else
        {
            rects[mergeIndex] = rightArr[rightIndex++];
            display(mergeIndex, 10);
            playSound(mergeIndex);
            mergeIndex++;
        }
    }

    // Remaining left array elements are placed in the rects array
    while (leftIndex < leftNum)
    {
        rects[mergeIndex] = leftArr[leftIndex++];
        display(mergeIndex, 10);
        playSound(mergeIndex);
        mergeIndex++;
    }

    // Remaining right array elements are placed in the rects array
    while (rightIndex < rightNum)
    {
        rects[mergeIndex] = rightArr[rightIndex++];
        display(mergeIndex, 10);
        playSound(mergeIndex);
        mergeIndex++;
    }
}

void mergeSort(int const start, int const end)
{
    // Base case
    if (start >= end) return;

    int mid = start + (end - start) / 2;
    mergeSort(start, mid);
    mergeSort(mid + 1, end);
    merge(start, mid, end);
}

// Best: O(nlogn), Worst: O(n^2), Average: O(nlogn)
void quickSort(int p, int r)
{
    if (p < r)
    {
        int q = partition(p, r);

        quickSort(p, q - 1);
        quickSort(q + 1, r);
    }
}

int partition(int p, int r)
{
    int pivot = rects[r];
    int i = (p - 1);

    for (int j = p; j <= r; j++)
    {
        if (rects[j] < pivot)
        {
            display(i, 10);
            swap(&rects[++i], &rects[j]);
            display(i, 10);
            playSound(i);
        }
    }

    display(i, 10);
    swap(&rects[i + 1], &rects[r]);
    display(i, 10);
    playSound(i);
    return i + 1;
}

void heapify(int n, int i)
{
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && rects[left] > rects[max]) max = left;
    if (right < n && rects[right] > rects[max]) max = right;

    if (max != i)
    {
        display(i, 10);
        swap(&rects[i], &rects[max]);
        display(i, 10);
        playSound(i);
        heapify(n, max);
    }
}

// Best: O(nlogn), Worst: O(nlogn), Average: O(nlogn)
void heapSort(int n)
{
    for (int i = n / 2 - 1; i >= 0; i--) {
        display(n, 10);
        playSound(n);
        heapify(n, i);
    }

    for (int j = n - 1; j > 0; j--)
    {
        display(j, 10);
        playSound(j);
        swap(&rects[0], &rects[j]);
        heapify(j, 0);
    }
}

// Best: O(n), Worst: O(n^2), Average: O(n^2)
void bubbleSort()
{
    int i, j;
    bool swapped;

    for (i = 0; i < 99; i++)
    {
        swapped = false;
        for (j = 0; j < 99 - i; j++)
        {
            if (rects[j] > rects[j + 1])
            {
                swap(&rects[j], &rects[j + 1]);
                display(j, 1);
                playSound(j);
                swapped = true;
            }
        }

        // If not swapped by inner loop 
        if (!swapped) break;
    }
}