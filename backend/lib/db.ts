import mysql from 'mysql2/promise';
import SQL from '@nearform/sql';

/**
 * Class representing database interactions.
 */
export class Database {
    #username: string;
    #password: string;
    #tietokanta: string;
    #host: string;
    #handle: mysql.Connection | undefined; // Connection handle, undefined if not connected

    /**
     * Initialize the Database class.
     */
    constructor() {
        this.#username = process.env.tietokanta_user ?? 'root';
        this.#password = process.env.tietokanta_password ?? 'password';
        this.#tietokanta = process.env.tietokanta ?? 'testi';
        this.#host = process.env.tietokanta_host ?? 'localhost';
        this.connect().then(_ => console.log('Connected to db'));
    }

    // Helper functions

    /**
     * Function to create a sleep delay.
     * @param {number} waitTimeInMs - The time to sleep in milliseconds.
     * @returns {Promise<void>} A promise that resolves after the sleep delay.
     */
    private sleep = (waitTimeInMs: number): Promise<void> => new Promise(resolve => setTimeout(resolve, waitTimeInMs));

    /**
     * Sanitize a SQL query to prevent SQL injection.
     * @param {string} query - The SQL query to sanitize.
     * @returns {string} The sanitized SQL query.
     */
    private sanitizeQuery = (query: string): string => SQL`${query}`.values[0];

    /**
     * Establish a database connection.
     * @returns {Promise<void>} A promise representing the connection process.
     */
    private connect = async (): Promise<void> => {
        this.#handle = await mysql.createConnection({
            host: this.#host,
            user: this.#username,
            password: this.#password,
            database: this.#tietokanta
        });
    }

    /**
     * Execute a SELECT query.
     * @param {string} query - The SELECT query to execute.
     * @returns {Promise<any[]>} A promise resolving to the query results.
     */
    private get = async (query: string): Promise<any[]> => {
        if (this.#handle === undefined) {
            await this.sleep(5000);
            return this.get(query);
        }
        return this.#handle.query(query);
    }

    /**
     * Execute a non-SELECT query.
     * @param {string} query - The query to execute.
     * @returns {Promise<any[]>} A promise resolving to the query results.
     */
    private exec = async (query: string): Promise<any[]> => {
        if (this.#handle === undefined) {
            await this.sleep(5000);
            return this.exec(query);
        }
        return this.#handle.execute(query);
    }

    // Database operations

    /**
     * Check if a user exists based on their card number.
     * @param {string} cardNumber - The user's card number.
     * @returns {Promise<boolean>} A promise resolving to true if the user exists, false otherwise.
     */
    async doesUserExist(cardNumber: string): Promise<boolean> {
        const [result] = await this.get(`SELECT * FROM card WHERE cardNumber = '${this.sanitizeQuery(cardNumber)}'`);
        return result.length > 0;
    }

    /**
     * Add a new user to the database.
     * @param {string} cardNumber - The user's card number.
     * @param {string} pin - The user's PIN.
     * @param {string} name - The user's name.
     * @param {string} address - The user's address.
     * @param {string} phone - The user's phone number.
     * @param {string} accountNumber - The user's account number.
     * @param {number} credit - The user's credit.
     * @param {number} balance - The user's balance.
     * @param {string} type - The user's account type.
     * @returns {Promise<boolean>} A promise resolving to true if the user is added successfully, false otherwise.
     */
    async addUser(cardNumber: string, pin: string, name: string, address: string, phone: string, accountNumber: string, credit: number, balance: number, type: string): Promise<boolean> {
        try {
            await this.exec(`
                INSERT INTO customer (customerName, customerAddress, customerPhonenumber)
                VALUES ('${this.sanitizeQuery(name)}', '${this.sanitizeQuery(address)}', '${this.sanitizeQuery(phone)}');
            `);

            await this.exec(`
                INSERT INTO account (accountNumber, accountType, accountBalance, creditLimit)
                VALUES ('${this.sanitizeQuery(accountNumber)}', '${this.sanitizeQuery(type)}', ${balance}, ${credit});
            `);

            await this.exec(`
                INSERT INTO card (idaccount, fidcustomer, cardNumber, cardPin)
                SELECT LAST_INSERT_ID(), LAST_INSERT_ID(), '${this.sanitizeQuery(cardNumber)}', '${this.sanitizeQuery(pin)}';
            `);

        } catch (error) {
            console.warn(error);
            return false;
        }
        return true;
    }

    /**
     * Delete a user from the database.
     * @param {string} cardNumber - The user's card number.
     * @returns {Promise<boolean>} A promise resolving to true if the user is deleted successfully, false otherwise.
     */
    async deleteUser(cardNumber: string): Promise<boolean> {
        try {
            await this.exec(`
                DELETE transactions, card, account, customer
                FROM (
                SELECT idaccount, fidcustomer
                FROM card WHERE cardNumber = '${this.sanitizeQuery(cardNumber)}') AS subquery
                LEFT JOIN transactions ON subquery.idaccount = transactions.idaccount
                LEFT JOIN card ON subquery.idaccount = card.idaccount
                LEFT JOIN account ON subquery.idaccount = account.idaccount
                LEFT JOIN customer ON subquery.fidcustomer = customer.idcustomer;
            `);
            return true;
        } catch (error) {
            console.warn(error);
            return false;
        }
    }

    /**
     * Get the hashed PIN of a user based on their card number.
     * @param {string} cardNumber - The user's card number.
     * @returns {Promise<string>} A promise resolving to the hashed PIN of the user.
     */
    getPasswordHash = async (cardNumber: string): Promise<string> => {
        if (!this.doesUserExist(cardNumber)) {
            console.log('User not found');
            return '';
        }

        const result = await this.get(`SELECT cardPin FROM card WHERE cardNumber = ${this.sanitizeQuery(cardNumber)}`);
        return result[0][0]?.cardPin;
    }
}
