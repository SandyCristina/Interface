import asyncio
import websockets
import matplotlib.pyplot as plt
import time
from tkinter import *
from threading import Thread

received_data = []
xlist = []
ylist = []
fig = plt.figure()
grafico = fig.add_subplot(1, 1, 1)
strat = time.time()

class Interface:
    def __init__(self):
        self.janela = Tk()
        self.janela.title("Interface")
        self.janelaTexto = Label(self.janela, text="Clique no botão para visualizar o gráfico")
        self.janelaTexto.grid(row=0, column=0)
        self.Button = Button(self.janela, text="Corrente", command=self.iniciar_conexao)
        self.Button.grid(row=1, column=0)
        self.corrente_disponivel = False
        self.conexao_aberta = False
        self.janela.mainloop()

    def plotgraph(self):
        grafico.clear()
        grafico.set_xlabel("Tempo (s)")
        grafico.set_ylabel("Valor de corrente, fase 1")
        grafico.set_title("Corrente")
        grafico.plot(xlist, ylist, color="blue")
        #plt.pause(0.01)

    def atualiza(self):
        xlist.append(time.time() - strat)
        if self.corrente_disponivel:
            data = received_data.pop(0)
            separa = data.split(",")
            for y in separa:
                if len(y) > 0:
                    ylist.append(float(y))
            self.plotgraph()
            print("Atualizando gráfico")
        else:
            print("Nenhum valor recebido da conexão")

    async def receber_dados_do_websocket(self):
        esp32_ip = "------ "
        port = 80
        url = f"ws://{esp32_ip}:{port}/ws"
        async with websockets.connect(url) as websocket:
            self.conexao_aberta = True
            while True:
                data = await websocket.recv()
                received_data.append(data)
                self.corrente_disponivel = True
                self.atualiza()
                print("Recebido:")
                print(data)

    async def fazer_conexao_websock(self):
        while True:
            try:
                await self.receber_dados_do_websocket()
            except websockets.ConnectionClosed:
                print("Conexão fechada. Tentando reconectar...")
                await asyncio.sleep(5)

    def iniciar_conexao(self):
        # Crie uma thread separada para executar o loop do asyncio
        thread = Thread(target=lambda: asyncio.run(self.fazer_conexao_websock()))
        thread.start()
        
if __name__ == "__main__":
    Interface()
