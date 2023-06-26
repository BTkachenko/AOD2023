#Author Bohdan Tkachenko 256630

using JuMP
import HiGHS
using DelimitedFiles

function read_data()
    data = readdlm("data3.txt", ',', Int, '\n', skipblanks=true)
    min_auto = data[1:3, 1:3]
    max_auto = data[4:6, 1:3]
    min_shift = data[7, 1:3]
    min_p = data[8, 1:3]

    return min_auto, max_auto, min_shift, min_p
end

function solve_police_cars(min_auto, max_auto, min_shift, min_p)
    model = Model(HiGHS.Optimizer)

    @variable(model, var[1:3, 1:3] >= 0)
    #ograniczenie ilosci
    @constraint(model, [i in 1:3, j in 1:3], min_auto[i, j] <= var[i, j] <= max_auto[i, j])
    #Liczba radiowozów w dzielnicach
    @constraint(model, [i in 1:3], sum(var[i, :]) >= min_p[i])
    #Liczba radiowozów w zmianach
    @constraint(model, [i in 1:3], sum(var[:, i]) >= min_shift[i])

    @objective(model, Min, sum(var))

    optimize!(model)

    return model
end

function display_results(model)
    print("\n")
    for i in 1:3, j in 1:3
        print(Int(value(model[:var][i, j])), " ")
        if j == 3
            print("\n")
        end
    end
    print("\n")
    println("Użyte radiowozy: ", Int(value(sum(model[:var]))))
end

function main()
    min_auto, max_auto, min_shift, min_p = read_data()
    model = solve_police_cars(min_auto, max_auto, min_shift, min_p)
    display_results(model)
end

main()
