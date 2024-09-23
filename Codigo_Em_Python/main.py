#Biblioteca de diretórios
import os

#Chama arquivo com calculo de luminosidade
import calculo_de_luz

# Nome do arquivo de entrada
arquivo_entrada = "dados.txt"

# Função para criar diretório se não existir
#---------------------------------------------v
def criar_diretorio_se_nao_existir(diretorio):
    if not os.path.exists(diretorio):
        os.makedirs(diretorio)
#---------------------------------------------^


# Função para processar o arquivo de entrada
#---------------------------------------------v
def processar_arquivo_dados(nome_arquivo):
    # Abrir o arquivo para leitura
    with open(nome_arquivo, 'r') as arquivo:
        linhas = arquivo.readlines()

    # Variáveis para guardar dados
    data = None
    dados = []

    # Iterar sobre as linhas do arquivo
    for linha in linhas:
        linha = linha.strip()
        if linha.startswith('20'):  # Linha da data
            # Se houver dados anteriores, processar e salvar
            if data and dados:
                salvar_dados(data, dados)
                dados = []  # Limpar lista de dados para a próxima data
            data = linha.replace('/', '')  # Formato aaaammdd
        elif linha and ':' in linha:  # Linha de dados válidos (hora, T, P, L)
            dados.append(linha)

    # Salvar os dados do último conjunto
    if data and dados:
        salvar_dados(data, dados)
#---------------------------------------------^


# Função para salvar os dados no arquivo correspondente à data
#---------------------------------------------v
def salvar_dados(data, dados):
    nome_arquivo = f"{data}.txt"
    with open(nome_arquivo, 'a') as arquivo:  # Abrir o arquivo em modo de adição ('a')
        for linha in dados:
            # Formatar cada linha de dado de acordo com o estilo especificado
            if linha.startswith('T:'):
                temperatura = linha.split(':')[1].strip()
                arquivo.write(f"Temperatura: {temperatura} graus Celsius\n")
            elif linha.startswith('P:'):
                pressao = linha.split(':')[1].strip()
                arquivo.write(f"Pressao: {pressao} hPa\n")
            elif linha.startswith('L:'):
                luz = linha.split(':')[1].strip()
                arquivo.write(f"Luz: {calculo_de_luz.calcula_lum(float(luz))}\n")
            else:
                arquivo.write(f"{linha}\n")
#---------------------------------------------^


# Verificar se o arquivo existe antes de processar
#---------------------------------------------v
if os.path.exists(arquivo_entrada):
    processar_arquivo_dados(arquivo_entrada)
    print(f"Arquivos foram criados com sucesso com base em {arquivo_entrada}.")
else:
    print(f"Erro: o arquivo {arquivo_entrada} não foi encontrado.")
#---------------------------------------------^

