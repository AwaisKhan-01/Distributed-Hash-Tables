# Security Policy

## Supported Versions

This project is currently in early development. Only the latest commit on the
`main` branch is supported.

| Version | Supported          |
| ------- | ------------------ |
| main    | :white_check_mark: |
| others  | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability, **please do not open a public
GitHub issue**. Instead:

1. Send a private report via GitHub's
   [Security Advisory](../../security/advisories/new) feature
   (Repository → Security → Advisories → New draft advisory).
2. Include:
   - A description of the vulnerability
   - Steps to reproduce it
   - Potential impact
   - Suggested fix (optional)

We aim to acknowledge reports within **72 hours** and to provide a fix or
mitigation within **14 days** for critical issues.

## Scope

This project is a local simulator and does not expose any network services by
default. Potential areas of concern include:

- **Buffer overflows** in BigInt or B-tree operations
- **Path traversal** in file-copy operations
- **Integer overflow** in identifier-space arithmetic

Thank you for helping keep this project secure!
