//
//  main.c
//  problema_15
//
//  Created by Alexandru Roșca on 29.01.2024.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//313AC Rosca Alexandru

struct product{
    char name[20];
    int quantity;
    float price;
};
typedef struct product product;

void addStock(void);
void newSale(void);
void showStock(void);
void updateStock(void);

int main(void)
{
    
    char c;
    
    do {
        printf("Menu: \n\tAdd stock (a)\n\tShow stock (s)\n\tNew sale (n)\n\tExit (x)\n");
        scanf("%c", &c);
        getchar();
        if(c == 'a')
            addStock();
        else if(c == 'n')
            newSale();
            else if(c == 's')
                showStock();
    } while (c != 'x');
    
    
    return 0;
}

void addStock(void)
{
    FILE *file = NULL;
    char c, ok = 'f';
    product product, checker[20];
    int size;
    
    do {
        printf("\nMenu:\n\tAdd a product (a)\n\tBack (x)\n");
        scanf("%c", &c);
        getchar();
        if(c == 'a')
        {
            printf("Enter product's name: ");
            fgets(product.name, 20, stdin);
            file = fopen("stock.bin", "rb");
            fseek(file, 0, SEEK_SET);
            size = (int)fread(checker, sizeof(product), 20, file);
            for(int i=0; i<size; i++)
            {
                if(!strcmp(product.name, checker[i].name))
                {
                    printf("Enter product's quantity: ");
                    scanf("%d", &product.quantity);
                    getchar();
                    checker[i].quantity += product.quantity;
                    ok = 't';
                }
            }
            file = fopen("stock.bin", "wb");
            fwrite(checker, sizeof(product), size, file);
            fclose(file);
            
            if(ok == 'f')
            {
                printf("Enter product's price: ");
                scanf("%f", &product.price);
                printf("Enter product's quantity: ");
                scanf("%d", &product.quantity);
                getchar();
                file = fopen("stock.bin", "ab");
                fseek(file, 0, SEEK_END);
                fwrite(&product, sizeof(product), 1, file);
                fclose(file);
            }
        }
    } while (c != 'x');
}

void newSale(void)
{
    FILE *file = NULL;
    file = fopen("stock.bin", "r+b");
    product products[20], sale[20];
    int cnt = 0, ok, size_prod = 0;
    float grand_total = 0;
    size_prod = (int)fread(products, sizeof(product), 20, file);
    char c, k;
    
    do {
        
        printf("Menu:\n\tBuy product (b)\n\tCheckout (c)\n");
        scanf("%c", &c);
        getchar();
        if(c == 'b')
        {
            ok = 0;
            printf("Enter product's name: ");
            fgets(sale[cnt].name, 20, stdin);
            
            for(int i=0; i<size_prod; i++)
                if(!strcmp(sale[cnt].name, products[i].name))
                {
                    printf("Enter desired quantity: ");
                    scanf("%d", &sale[cnt].quantity);
                    if(sale[cnt].quantity > products[i].quantity)
                    {
                        printf("The requested quantity is not available! The max available is %d", products[i].quantity);
                        printf("\nAdd new quantity (a)\nCancel (c)");
                        getchar();
                        scanf("%c", &k);
                        getchar();
                        if(k == 'a')
                        {
                            printf("Enter quantity: ");
                            scanf("%d", &sale[cnt].quantity);
                            getchar();
                            ok = 1;
                            sale[cnt].price = products[i].price * sale[cnt].quantity;
                            cnt++;
                        }
                    }
                    else
                    {
                        getchar();
                        ok = 1;
                        sale[cnt].price = products[i].price * sale[cnt].quantity;
                        cnt++;
                    }
                }
            if(!ok)
                printf("Sorry! Your request is unavailable!\n");
        
        }
    } while (c != 'c');
    
    for(int i=0; i<cnt; i++)
    {
        grand_total += sale[i].price;
        for(int j=0; j<size_prod; j++)
            if(!strcmp(sale[i].name, products[j].name))
            {
                products[j].quantity -= sale[i].quantity;
            }
    }
    fseek(file, 0, 0);
    fwrite(products, sizeof(product), size_prod, file);
    printf("Your grand total is %.2f\n", grand_total);
    fclose(file);
    updateStock();
}

void showStock(void)
{
    FILE *file = NULL;
    file = fopen("stock.bin", "rb");
    fseek(file, 0, SEEK_SET);
    product products[20];
    int size_prod;
    
    size_prod = (int)fread(products, sizeof(product), 20, file);
    
    for(int i=0; i<size_prod; i++)
        printf("%d.\n\tName: %s\tPrice: %.2f\n\tQuantity: %d\n", i+1, products[i].name, products[i].price, products[i].quantity);
    fclose(file);
}

void updateStock(void)
{
    FILE *file = fopen("stock.bin", "rb");
    product checker[20];
    int size;
    
    size = (int)fread(checker, sizeof(product), 20, file);
    
    for(int i=0; i<size; i++)
    {
        if(checker[i].quantity == 0)
        {
            for(int j = i; j<size-1; j++)
                checker[j] = checker[j+1];
            i--;
            size--;
        }
    }
    file = fopen("stock.bin", "wb");
    fseek(file, 0, SEEK_SET);
    fwrite(checker, sizeof(product), size, file);
    fclose(file);
}
