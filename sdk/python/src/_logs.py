#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

logsCmd = "logs"
logsPos = "transactions"
logsFmt = "json"
logsOpts = {
    "emitter": {"hotkey": "-m", "type": "flag"},
    "topic": {"hotkey": "-B", "type": "flag"},
    "articulate": {"hotkey": "-a", "type": "switch"},
    "raw": {"hotkey": "-w", "type": "switch"},
    "cache": {"hotkey": "-o", "type": "switch"},
    "fmt": {"hotkey": "-x", "type": "flag"},
    "verbose:": {"hotkey": "-v", "type": "switch"},
    "help": {"hotkey": "-h", "type": "switch"},
}

def logs(self):
    ret = self.toUrl(logsCmd, logsPos, logsFmt, logsOpts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text

