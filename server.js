const express = require('express');
const http_ = require('http');
const socketio = require('socket.io');

const app = express();
const http = http_.createServer(app);
const io = socketio(http);

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/hearts.html');
});

app.use(express.static('public', {maxage: '0h'}));

const two = 0;
const three = 1;
const four = 2;
const five = 3;
const six = 4;
const seven = 5;
const eight = 6;
const nine = 7;
const ten = 8;
const jack = 9;
const queen = 10;
const king = 11;
const ace = 12;

const clubs = 0;
const diams = 1;
const spades = 2;
const hearts = 3;

function cardAsHtmlString({rank, suit})
{
    const suitNames = ['clubs', 'diams', 'spades', 'hearts'];
    const suitGlyphs = ['&clubs;', '&diams;', '&spades;', '&hearts;']
    const rankNames = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A'];

    const R = rankNames[rank];
    const S = suitNames[suit];
    const G = `&${S};`;
    const s = `<li><a class="card rank-${R.toLowerCase()} ${S}" href="#"><span class="rank">${R}</span><span class="suit">${G}</span></a></li>`;
    return s;
}

function deal()
{
    const array = [];
    for (let i=0; i<52; i++)
    {
        let suit = Math.floor(i / 13);
        let rank = i % 13;
        array.push({rank, suit});
    }

    var currentIndex = array.length, temporaryValue, randomIndex;

    // While there remain elements to shuffle...
    while (0 !== currentIndex) {

      // Pick a remaining element...
      randomIndex = Math.floor(Math.random() * currentIndex);
      currentIndex -= 1;

      // And swap it with the current element.
      temporaryValue = array[currentIndex];
      array[currentIndex] = array[randomIndex];
      array[randomIndex] = temporaryValue;
    }

    return array;
}

function giveCardsTo(player, cards)
{
    for (card of cards)
    {
        let html = cardAsHtmlString(card);
        io.emit('addcard', {html, player});
    }
}

function cardToOrd({rank, suit})
{
    return suit*13 + rank;
}

function distribute(deck)
{
    const compare = (a, b) => cardToOrd(a) - cardToOrd(b);
    function sort(hand)
    {
        return hand.sort(compare);
    }
    giveCardsTo('north', sort(deck.slice(0,13)));
    giveCardsTo('east', sort(deck.slice(13,26)));
    giveCardsTo('south', sort(deck.slice(26,39)));
    giveCardsTo('west', sort(deck.slice(39,52)));
}

io.on('connection', socket => {
    console.log('a user connected');
    socket.on('disconnect', () => {
        console.log('user disconnected');
    });

    const deck = deal();
    distribute(deck);
});

http.listen(3000, () => {
    console.log('listening on *:3000');
});
