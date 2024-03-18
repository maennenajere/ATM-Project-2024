import express from 'express'
import router  from './routes/routes';

const port = 3000
const app = express()

app.use(router);

app.listen(port, () => {
    console.log(`Listening on port ${port}`)
})