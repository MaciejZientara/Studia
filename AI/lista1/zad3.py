import random

# 1. royal flush
# 2. straight flush
# 3. four of a kind
# 4. full house
# 5. flush
# 6. straight
# 7. three of a kind
# 8. two pair
# 9. one pair
# 10. high card

# possible sets:
# four of a kind, full house, three of a kind, two pair, one pair
# 3, 4, 7, 8, 9

# black spades (♠), red hearts (♥), blue diamonds (♦) and green clubs (♣)
colors = ['♠','♥','♦','♣']
figures = ['A','K','Q','J']
number = ['2','3','4','5','6','7','8','9','10']

def drawHandFigures():
    res = []
    while len(res)!=5:
        tmp = random.choice(figures) + random.choice(colors)
        if tmp not in res:
            res.append(tmp)
    return sorted(res)

def whatSet(hand):
    figuresCount = [0,0,0,0]
    for card in hand:
        figuresCount[figures.index(card[0])]+=1
    if 4 in figuresCount:
        # print('four of a kind')
        return 3
    if 3 in figuresCount and 2 in figuresCount:
        # print('full house')
        return 4
    if 3 in figuresCount:
        # print('three of a kind')
        return 7
    pairCount = sum(x==2 for x in figuresCount)
    if pairCount == 2:
        # print('two pair')
        return 8
    if pairCount == 1:
        # print('one pair')
        return 9
    

customDeck = [  '2♠','3♠','4♠','5♠','6♠','7♠','8♠','9♠','10♠',
                '2♥','3♥','4♥','5♥','6♥','7♥','8♥','9♥','10♥',
                '2♦','3♦','4♦','5♦','6♦','7♦','8♦','9♦','10♦',
                '2♣','3♣','4♣','5♣','6♣','7♣','8♣','9♣','10♣']

def calculateChance(hand):
    colorsCount = [0,0,0,0]
    numberCount = [0,0,0,0,0,0,0,0,0]
    
    for card in customDeck:
        for c in colors:
            if c in card:
                colorsCount[colors.index(c)]+=1
        for n in number:
            if n in card:
                numberCount[number.index(n)]+=1
    print('colorsCount =', colorsCount, 'numberCount =', numberCount)
    chances = [0,0,0,0,0,0,0,0,0,0]
    chances[0] = 0 #royal flush

    #                                                                               TODO  !!!

    chances[10] = 0 #high card

    return sum(chances[i] for i in range(whatSet(hand))) # suma szans lepszych ustawien niz przeciwnika


hand = drawHandFigures()
print(hand)
print(whatSet(hand))
print(calculateChance(hand))
