import { randomBytes, createHmac } from 'crypto';

// Interface to define the structure of user data
interface UserData {
    cookie: string; // Holds the user's cookie
    ttl: number; // Time to live for the cookie
    salt: string; // Salt used to secure the cookie
}

/**
 * Class representing authentication functionalities.
 */
export class Auth {
    #storage: Map<string, UserData>; // Map to hold valid sessions
    #secret: string; // Secret used to secure cookies
    #ttl: number; // Time to live for the cookies

    /**
     * Initialize the Auth class.
     * @param {string} secret - The secret used to secure cookies.
     * @param {number} ttl - Time to live for the cookies.
     */
    constructor(secret: string, ttl: number) {
        console.log(`Setting up auth storage with ttl of ${ttl}`);
        this.#storage = new Map<string, UserData>();
        this.#secret = secret;
        this.#ttl = ttl;
    }

    // Helper functions

    private getStorage = (userId: string) => this.#storage.get(userId);
    private delField = (userId: string) => this.#storage.delete(userId);
    private updateStorage = (userId: string, data: UserData) => this.#storage.set(userId, data);
    private userIdFromCookie = (cookie: string): string => cookie.split('.')[0];

    /**
     * Generate a cookie.
     * @param {string} userId - The user ID.
     * @param {string} salt - The salt used to secure the cookie.
     * @returns {string} The generated cookie.
     */
    private genCookie = (userId: string, salt: string) => {
        const cookie = this.getIdentifier(userId);
        const signature = this.generateSignature(cookie, salt);
        return `${cookie}.${signature}`;
    }

    /**
     * Get the base64 encoded identifier for a user.
     * @param {string} userId - The user ID.
     * @returns {string} The base64 encoded identifier.
     */
    private getIdentifier = (userId: string): string => {
        const keyBuffer = Buffer.from(userId);
        return keyBuffer.toString('base64');
    }

    /**
     * Generate a signature using HMAC and SHA256.
     * @param {string} data - The data to sign.
     * @param {string} salt - The salt used in the signature.
     * @returns {string} The generated signature.
     */
    private generateSignature = (data: string, salt: string): string => {
        const hmac = createHmac('sha256', this.#secret);
        hmac.update(data + salt);
        return hmac.digest('base64');
    }

    /**
     * Verify the signature of a cookie.
     * @param {string} cookie - The cookie to verify.
     * @param {string} salt - The salt used in the signature.
     * @returns {boolean} True if the signature is valid, false otherwise.
     */
    private verifySignature = (cookie: string, salt: string): boolean => {
        const [data, signature] = cookie.split('.');
        const expectedSignature = this.generateSignature(data, salt);
        return signature === expectedSignature;
    }

    /**
     * Automatically delete expired cookies.
     * @param {string} userId - The user ID.
     */
    private cleaner = (userId: string) => {
        setTimeout(() => {
            this.delField(userId);
        }, this.#ttl);
    }

    /**
     * Check if a user is logged in based on their cookie.
     * @param {string} cookie - The user's cookie.
     * @returns {boolean} True if the user is logged in, false otherwise.
     */
    isUserLoggedin = (cookie: string): boolean => {
        const userId = this.userIdFromCookie(cookie);
        const userData = this.getStorage(userId);

        if (!userData) {
            return false;
        }

        const { ttl, salt } = userData;

        if (ttl < Date.now()) {
            this.delField(userId);
            return false;
        }

        if (!this.verifySignature(cookie, salt)) {
            return false;
        }

        return true;
    }

    /**
     * Log a user in and return the generated cookie.
     * @param {string} userId - The user ID.
     * @returns {string} The generated cookie.
     */
    setLoggedIn = (userId: string): string => {
        const salt = randomBytes(16).toString('hex');
        const cookie = this.genCookie(userId, salt);
        const encodedId = Buffer.from(userId).toString('base64');

        const userData: UserData = {
            cookie: cookie,
            ttl: Date.now() + this.#ttl,
            salt: salt
        }

        this.updateStorage(encodedId, userData);
        this.cleaner(encodedId);
        return cookie;
    }

    /**
     * Log a user out.
     * @param {string} cookie - The user's cookie.
     */
    setLoggedOut = (cookie: string) => {
        this.delField(this.userIdFromCookie(cookie));
        return;
    }
}
