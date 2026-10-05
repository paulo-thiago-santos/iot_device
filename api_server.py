#.\venv\Scripts\python.exe -m pip install fastapi uvicorn
#.\venv\Scripts\python.exe -m uvicorn api_server:app --host 0.0.0.0 --port 8000
# http://localhost:8000/contador

from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

contador = 0


class Contador(BaseModel):
    valor: int


@app.get("/contador")
def get_contador():
    global contador

    contador += 1

    return {
        "contador": contador
    }


@app.post("/contador")
def post_contador(dados: Contador):
    global contador

    contador = dados.valor

    return {
        "contador": contador
    }
