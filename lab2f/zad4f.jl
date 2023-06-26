#Author Bohdan Tkachenko 256630


using JuMP
using Random
import HiGHS
import DelimitedFiles
using DelimitedFiles

function read_containers_data()
    conteners = readdlm("data4.txt", ',', Int, '\n', skipblanks=true)
    return conteners
end

function solve_containers(conteners, m, n, k)
    model = Model(HiGHS.Optimizer)

    @variable(model, var[1:m, 1:n], Bin)
    #każde pole może zawierać albo kontener albo kamere
    @constraint(model, [i in 1:m, j in 1:n], var[i, j] + conteners[i, j] <= 1)
    # Kontener musi byc obserwowany przez kamere
    for i in 1:m, j in 1:n; conteners[i, j] == 1
        col_range = max(1, i - k):min(m, i + k)
        row_range = max(1, j - k):min(n, j + k)
        @constraint(model, (sum(var[i, row_range]) + sum(var[col_range, j])) >= 1)
    end

    @objective(model, Min, sum(var))

    optimize!(model)

    return model
end

function display_results(model)
    println("\nUżytych lamp: ", round(Int, objective_value(model)))
    println("Rozstawienie lamp:")
    display(round.(Int, value.(model[:var])))
end

function main()
    conteners = read_containers_data()
    m, n, k = size(conteners, 1), size(conteners, 2), 3
    model = solve_containers(conteners, m, n, k)
    display_results(model)
end

main()