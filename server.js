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

function giveCardsTo(id, cards)
{
    for (card of cards)
    {
        io.emit('addcard', {id, card});
    }
}

function showCard(id, card)
{
    io.emit('showcard', {id, card})
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
    giveCardsTo('human', sort(deck.slice(0,13)));
    showCard('west', null);
    showCard('north', deck[14]);
    showCard('east', deck[15]);
    giveCardsTo('played', sort(deck.slice(15,18)));
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
