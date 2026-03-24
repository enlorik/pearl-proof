'use strict';

const express = require('express');
const cors = require('cors');
const rateLimit = require('express-rate-limit');
const { execFile } = require('child_process');
const { randomUUID } = require('crypto');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

const runLimiter = rateLimit({
  windowMs: 60 * 1000,
  max: 30,
  standardHeaders: true,
  legacyHeaders: false,
});

app.get('/health', (_req, res) => {
  res.json({ status: 'ok' });
});

app.post('/run', runLimiter, async (req, res) => {
  const { code } = req.body || {};

  if (typeof code !== 'string' || code.trim().length === 0) {
    return res.status(400).json({ error: 'code must be a non-empty string' });
  }

  if (code.length > 2000) {
    return res.status(400).json({ error: 'code exceeds maximum length of 2000 characters' });
  }

  const tmpFile = path.join('/tmp', `${randomUUID()}.proof`);

  try {
    fs.writeFileSync(tmpFile, code);

    const output = await new Promise((resolve, reject) => {
      execFile('/app/pearlproof', [tmpFile], { timeout: 5000, maxBuffer: 1024 * 1024 }, (err, stdout, stderr) => {
        if (err && err.killed) {
          return reject(new Error('Execution timed out'));
        }
        resolve(stdout || stderr || '');
      });
    });

    return res.json({ output });
  } catch (err) {
    if (err.message === 'Execution timed out') {
      return res.status(500).json({ error: 'Execution timed out' });
    }
    return res.status(500).json({ error: 'Unexpected server error' });
  } finally {
    try {
      fs.unlinkSync(tmpFile);
    } catch (_) {
      // ignore cleanup errors
    }
  }
});

app.listen(PORT, () => {
  console.log(`PearlProof server listening on port ${PORT}`);
});
