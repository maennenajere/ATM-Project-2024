import express from 'express'
import bcrypt from 'bcrypt';
import { Database } from '../lib/db'
import { Auth } from '../lib/userAuthHandler'

const router = express.Router()
const saltRounds = 10;
const tietokanta = new Database()
const auth = new Auth('you will never guess this', 1*1000*60)

router.get('/', function (_req, res) {
  res.send('Hello World')
  res.end()
})

router.post('/login', async (req, res) => {

  const data = JSON.parse(await new Promise<string>(resolve => req.on('data', raw => resolve(raw.toString()))))

  const username = data?.username
  const password = data?.password

  if (!username || !password)
    return res.send('Missing params')

  const passwdhash = await tietokanta.getPasswordHash(username)
 
  if (!passwdhash) 
    return res.send('Bad login')

  const verify = bcrypt.compareSync(password, passwdhash)

  if (!verify)
      return res.send('Bad login')
  
  console.log('logging in...')

  const cookie = auth.setLoggedIn(username)

  console.log(`User ${username} logged in`)
  res.send(cookie)
  res.end()
})
 
router.get('/balance/:cardNumber', async (req, res) => {
  const cookie = req.headers.cookie;

  if (!cookie) {
    return res.send('Not logged in');
  }

  if (!auth.isUserLoggedin(cookie)) {
    return res.send('Not logged in');
  }

  const cardNumber = req.params.cardNumber;

  if (!cardNumber) {
    return res.send('Card number is missing');
  }

  const balance = await tietokanta.getUserBalance(cardNumber);

  if (balance === null) {
    return res.send('User not found');
  }

  res.send(balance);
});

router.get('/transactions/:cardNumber', async (req, res) => {
  const cookie = req.headers.cookie;

  if (!cookie) {
    return res.send('Not logged in');
  }

  if (!auth.isUserLoggedin(cookie)) {
    return res.send('Not logged in');
  }

  const cardNumber = req.params.cardNumber;

  if (!cardNumber) {
    return res.send('Card number is missing');
  }

  const balance = await tietokanta.getUserTransactions(cardNumber);

  if (balance === null) {
    return res.send('User not found');
  }

  res.send(balance);
});

router.post('/register', async (req, res) => {

  const data = JSON.parse(await new Promise<string>(resolve => req.on('data', raw => resolve(raw.toString()))))

  const username = data?.username
  const password = data?.password
  const accNumber = data?.accNumber
  const name = data?.name
  const address = data?.address
  const phone = data?.phone
  const type = data?.type

  if (!username || !password || !name || !address || !phone || !type)
    return res.send('Missing params')

  const exists = await tietokanta.doesUserExist(username)

  if (exists)
    return res.send('User already exists')
    bcrypt.hash(password, saltRounds, async (err, hash) => {
      if (err) throw err   
      const result = await tietokanta.addUser(username, hash, name, address, phone, accNumber, 420, 1337, type)
      if (!result) return res.send('Failed')
      console.log(`Card: ${username} created`)
      res.send('Registered successfully')
   });
})


router.post('/delete', async (req, res) => {

  const data = JSON.parse(await new Promise<string>(resolve => req.on('data', raw => resolve(raw.toString()))))

  const username = data?.username
  const password = data?.password

  if (!username || !password)
    return res.send('Missing params')

  const exists = await tietokanta.doesUserExist(username)
  
  if (!exists)
      return res.send('Bad login')

  const hash = await tietokanta.getPasswordHash(username)
  console.log(`Card: ${username} pin hash: ${hash}`)

  const verify = bcrypt.compareSync(password, hash)

  if (!verify)
      return res.send('Bad login')

  await tietokanta.deleteUser(username)
  console.log(`Card: ${username} deleted`)
  res.send('Deleted successfully')
})

router.get('/test', (req, res) => {

  const cookie = req.headers.cookie

  if (!cookie)
    return res.send('Not logged in')
  if (!auth.isUserLoggedin(cookie))
    return res.send('Not logged in')

  res.send('You are logged in!')
})



// transfer money. from is used for testing, prod version could for example just get the data from db using cookie
router.get('/transfer/:amount/:from/:to', (req, res) => {

  const cookie = req.headers.cookie

  if (!cookie)
    return res.send('Not logged in')
  if (!auth.isUserLoggedin(cookie))
    return res.send('Not logged in')

  const value = Number(req.params.amount)

  if (isNaN(value))
    return res.send('Not a number')

  if (value > 100 && value === 50 ? false : value % 20 !== 0)
    return res.send('Only 20, 40, 50 or 100')

  const from = req.params.from
  const to = req.params.to

  if (!from || !to) // also implement check to see if those are valid
    return res.send('Invalid params')

  // implement actual logic for transfer

  res.send('Transfer done')
})

router.get('/logout', (req, res) => {

  const cookie = req.headers.cookie

  if (!cookie)
    return res.send('Not logged in')
  if (!auth.isUserLoggedin(cookie))
    return res.send('Not logged in')

  auth.setLoggedOut(cookie)

  res.send('You are logged out!')
})

export default router;
